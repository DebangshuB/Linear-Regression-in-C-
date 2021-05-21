#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int get_attr(char line[])
{
    int count = 0;

    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == ',')
            count++;
    }

    return (++count);
}

int dimensions(char inp_file_name[], int dimen[])
{
    ifstream fin(inp_file_name);

    if (!fin)
    {
        return 1;
    }

    char line[101];
    int attributes = 0;
    int tuples = 0;

    while (!fin.eof())
    {
        fin.getline(line, 100);

        if (tuples == 0)
        {
            attributes = get_attr(line);
        }

        tuples++;
    }

    fin.close();

    tuples--;

    dimen[0] = tuples;
    dimen[1] = attributes;

    return 0;
}

int place_headers(char inp[], string headers[])
{
    ifstream fin(inp);

    char line[101];

    fin.getline(line, 100);
    int idx = 0;

    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == ',')
        {
            idx++;
            i++;
        }

        headers[idx].push_back(line[i]);
    }

    fin.close();

    return 0;
}

int place_tuples(char inp[], int **tuples, int rows, int cols)
{
    ifstream fin(inp);

    char line[101];
    int curr = 0;
    int digit;
    int fac = 10;
    int r = 0, c = 0;

    fin.getline(line, 100); // Leave the headers

    while (!fin.eof())
    {
        fin.getline(line, 100);

        for (int i = 0;; i++)
        {
            if (line[i] != ',' && line[i] != '\0')
            {
                digit = line[i] - '0';
                curr = curr * fac + digit;
            }
            else
            {
                tuples[r][c++] = curr;

                if (c >= cols)
                {
                    c = 0;
                    r++;
                }

                curr = 0;
            }

            if (line[i] == '\0')
                break;
        }
    }

    fin.close();

    return 0;
}

void display(int dimen[], string headers[], int **tuples)
{
    cout << dimen[0] << "::" << dimen[1] << endl;

    for (int i = 0; i < dimen[1]; ++i)
    {
        cout << headers[i] << endl;
    }

    for (int i = 0; i < dimen[0]; ++i)
    {
        for (int j = 0; j < dimen[1]; ++j)
        {
            cout << tuples[i][j] << " ";
        }
        cout << endl;
    }
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void shuffle(int **tuples, int rows, int cols)
{
    int times = rand() % rows;
    int temp;

    while (times--)
    {
        int a = rand() % rows;
        int b = rand() % rows;

        for (int i = 0; i < cols; i++)
        {
            swap(tuples[a][i], tuples[b][i]);
        }
    }
}

int ttsplit(int per, int rows)
{
    return (rows * (100 - per) / 100);
}

double calc_delta(double wts[], int **tuples, int row, int cols)
{
    double pred;

    pred = wts[0] * 1;
    for (int i = 0; i < cols - 1; i++)
    {
        pred += (wts[i + 1] * tuples[row][i]);
    }
    return (double)(pred - tuples[row][cols - 1]);
}

void LinearRegression(int **tuples, int rows, int cols, double wts[])
{
    double delta;
    double avg_delta, sum_delta;
    int batch_size = 2;
    double learning_rate = 0.00001;
    double MSE;
    int epochs = 5;

    for (int i = 0; i < cols; i++)
    {
        wts[i] = 1;
    }

    int e = 0;
    while (e != epochs)
    {
        MSE = 0;
        for (int i = 0; i < rows;)
        {
            sum_delta = 0;

            for (int ii = 0; ii < batch_size; ii++, i++)
            {
                delta = calc_delta(wts, tuples, i, cols);
                sum_delta += delta;
                MSE += delta * delta;
            }

            avg_delta = sum_delta / (double)(batch_size);
            avg_delta = avg_delta * learning_rate;

            wts[0] = wts[0] - avg_delta;
            for (int idx = 1; idx < cols - 1; idx++)
            {
                wts[idx] = wts[idx] - (avg_delta * tuples[i][idx]);
            }
        }

        //cout << "Epoch No : " << e + 1 << " : " << MSE / ((double)rows / (double)batch_size) << endl;
        e++;
    }
}

void check(int **tuples, double wts[], int split, int rows, int cols)
{
    double pred;
    double error = 0;

    for (int i = split; i < rows; i++)
    {
        pred = wts[0] * 1;

        for (int ii = 0; ii < cols - 1; ii++)
        {
            pred += (wts[ii + 1] * tuples[i][ii]);
        }
        cout << pred << " : " << tuples[i][cols - 1] << endl;
        error += abs((double)(pred - tuples[i][cols - 1])) / tuples[i][cols - 1] * 100;
    }
    error = error / (double)(rows - split);
    cout << "Accuracy : " << 100.00 - error << "%";
}

int main()
{
    char inp_file_name[] = "mlr03.csv";
    int dimen[2];

    dimensions(inp_file_name, dimen);

    string headers[dimen[1]]; // Stores the column names

    // Stores the data row-wise

    int **tuples = new int *[dimen[0]];

    for (int i = 0; i < dimen[0]; i++)
        tuples[i] = new int[dimen[1]];

    place_headers(inp_file_name, headers);

    place_tuples(inp_file_name, tuples, dimen[0], dimen[1]);

    //display(dimen, headers, tuples);
    shuffle(tuples, dimen[0], dimen[1]);

    int split = ttsplit(20, dimen[0]);

    double weights[dimen[1]]; // One less as the targets are in the same array and one more for the bias term

    LinearRegression(tuples, split, dimen[1], weights);

    // for (int i = 0; i < dimen[1]; i++)
    // {
    //     cout << weights[i] << endl;
    // }

    check(tuples, weights, split, dimen[0], dimen[1]);
    return 0;
}
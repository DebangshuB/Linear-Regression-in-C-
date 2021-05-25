#include <fstream>
#include <cmath>

using namespace std;

//-------------------------Internal----------------------------

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

int get_attr(char data_line[])
{
    int count = 0;

    for (int i = 0; data_line[i] != '\0'; i++)
    {
        if (data_line[i] == ',')
            count++;
    }

    return (++count);
}

//-------------------------External----------------------------

void get_dimensions(char input_file[], int &row_size, int &column_size)
{
    ifstream fin(input_file);

    if (!fin)
    {
        return;
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

    row_size = tuples;
    column_size = attributes;
}

int load_data(char input_file[], double **data, int row_size, int column_size)
{
    ifstream fin(input_file);

    char line[101];
    double curr = 0.0;
    double digit = 0.0;
    double fac = 10.0;
    double decimal_fac = 10.0;
    bool decimal_flag = false;
    int r = 0, c = 0;

    fin.getline(line, 100); // Leave the headers

    while (!fin.eof())
    {

        fin.getline(line, 100);

        for (int i = 0;; i++)
        {

            if (line[i] != ',' && line[i] != '\0')
            {
                if (line[i] == '.')
                {
                    decimal_flag = true;
                    continue;
                }

                digit = (double)(line[i] - '0');

                if (decimal_flag)
                {
                    curr = curr + digit / decimal_fac;
                    decimal_fac *= 10.0;
                }
                else
                {
                    curr = curr * fac + digit;
                }
            }
            else
            {
                data[r][c++] = curr;

                if (c >= column_size)
                {
                    c = 0;
                    r++;
                }
                decimal_flag = false;
                decimal_fac = 10;
                curr = 0.0;
            }

            if (line[i] == '\0')
                break;
        }
    }

    fin.close();

    return 0;
}

void shuffle_data(double **data, int row_size, int column_size, unsigned int seed)
{
    int times = rand() % row_size;
    srand(seed);

    while (times--)
    {
        int a = rand() % row_size;
        int b = rand() % row_size;

        for (int i = 0; i < column_size; i++)
        {
            swap(data[a][i], data[b][i]);
        }
    }
}

int train_test_split(double percentage, int row_size)
{
    return (row_size * (100 - percentage) / 100);
}

void min_max_scaler(double **data, int row_size, int column_size)
{
    double min, max;

    for (int col = 0; col < column_size; col++)
    {
        min = 1e9;
        max = -1e9;

        for (int i = 0; i < row_size; i++)
        {
            if (data[i][col] < min)
                min = data[i][col];

            if (data[i][col] > max)
                max = data[i][col];
        }

        for (int i = 0; i < row_size; i++)
        {
            data[i][col] = (data[i][col] - min) / (max - min);
        }
    }
}

void standard_scaler(double **data, int row_size, int column_size)
{
    double mean, var, sum, sum2, std;

    for (int col = 0; col < column_size; col++)
    {
        sum = 0.0;
        sum2 = 0.0;

        for (int i = 0; i < row_size; i++)
        {
            sum += data[i][col];
            sum2 += data[i][col] * data[i][col];
        }

        mean = sum / (double)row_size;
        var = (sum2 / (double)row_size) - (mean * mean);
        std = sqrt(var);

        for (int i = 0; i < row_size; i++)
        {
            data[i][col] = (data[i][col] - mean) / std;
        }
    }
}

#include <iostream>
#include <fstream>
#include <iomanip>

#include "preprocessing.h"

using namespace std;

void display(int dimen[], double **tuples)
{
    //cout << dimen[0] << "::" << dimen[1] << endl;
    cout << endl
         << endl;
    for (int i = 0; i < dimen[0]; ++i)
    {
        for (int j = 0; j < dimen[1]; ++j)
        {
            cout << std::setprecision(precision) << std::fixed << tuples[i][j] << " ";
        }
        cout << endl;
    }
}

double calc_delta(double wts[], double **tuples, int row, int cols)
{
    double pred;

    pred = wts[0] * 1;
    for (int i = 0; i < cols - 1; i++)
    {
        pred += (wts[i + 1] * tuples[row][i]);
    }
    return (double)(pred - tuples[row][cols - 1]);
}

void LinearRegression(double **tuples, int rows, int cols, double wts[])
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

void check(double **tuples, double wts[], int split, int rows, int cols)
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
    char input_file[] = "mlr03.csv";
    int dimen[2];

    get_dimensions(input_file, dimen);

    // Stores the data row-wise

    double **tuples = new double *[dimen[0]];

    for (int i = 0; i < dimen[0]; i++)
        tuples[i] = new double[dimen[1]];

    place_tuples(input_file, tuples, dimen[0], dimen[1]);

    //shuffle(tuples, dimen[0], dimen[1]);

    //int split = ttsplit(20, dimen[0]);

    display(dimen, tuples);

    //double weights[dimen[1]]; // One less as the targets are in the same array and one more for the bias term

    //LinearRegression(tuples, split, dimen[1], weights);

    // for (int i = 0; i < dimen[1]; i++)
    // {
    //     cout << weights[i] << endl;
    // }

    //check(tuples, weights, split, dimen[0], dimen[1]);
    return 0;
}
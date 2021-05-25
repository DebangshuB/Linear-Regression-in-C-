#include <iostream>
#include <fstream>
#include <iomanip>

#define precision 7

#include "preprocessing.h"
#include "LinearRegression.h"

using namespace std;

void display(int row_size, int column_size, double **data)
{
    cout << endl
         << endl;

    cout << "Rows : " << row_size << endl;
    cout << "Columns : " << column_size << endl;

    cout << endl
         << endl;

    for (int i = 0; i < row_size; ++i)
    {
        for (int j = 0; j < column_size; ++j)
        {
            cout << std::setprecision(precision) << std::fixed << data[i][j] << " ";
        }
        cout << endl;
    }
}

void check(double **data, double weights[], int split, int row_size, int column_size)
{
    double pred;
    double error = 0.0;
    double tot_error = 0.0;

    for (int i = split; i < row_size; i++)
    {
        pred = weights[0] * 1;

        for (int ii = 0; ii < column_size - 1; ii++)
        {
            pred += (weights[ii + 1] * data[i][ii]);
        }

        error = (abs((double)(pred - data[i][column_size - 1])) / data[i][column_size - 1]) * 100;
        tot_error += error;
        cout << setw(precision) << pred << " : " << setw(precision) << data[i][column_size - 1] << " " << setw(precision) << error << endl;
    }

    cout << endl
         << "Accuracy : " << (100.0 - tot_error / (double)(row_size - split))
         << endl;
}

int main()
{
    char input_file[] = "input.csv";
    int column_size;
    int row_size;

    get_dimensions(input_file, row_size, column_size);

    cout << endl
         << endl;

    cout << "Rows : " << row_size << endl;
    cout << "Columns : " << column_size << endl;

    cout << endl
         << endl;

    double **data = new double *[row_size];
    for (int i = 0; i < row_size; i++)
        data[i] = new double[column_size];

    load_data(input_file, data, row_size, column_size);

    for (int i = 0; i < row_size; i++)
        data[i][column_size - 1] *= 1;

    shuffle_data(data, row_size, column_size, 42);

    //min_max_scaler(data, row_size, column_size);

    int split = train_test_split(20.0, row_size);

    //display(row_size, column_size, data);

    //Linear Regression

    double weights[column_size]; // One less as the targets are in the same array and one more for the bias term
    LinearRegression(data, weights, split, column_size, 10, 0.00001, 100);

    cout << endl
         << "Weights : " << endl;

    for (int i = 0; i < column_size; i++)
    {
        cout << "W" << i << " " << setw(7) << weights[i] << endl;
    }

    cout << endl
         << endl;

    check(data, weights, split, row_size, column_size);

    return 0;
}
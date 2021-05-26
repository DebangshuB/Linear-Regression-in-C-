#include <iostream>
using namespace std;

void LinearRegression(double **data, double weights[], int row_size, int column_size, int batch_s, double lr, int ep)
{
    int batch_size = batch_s;
    int epochs = ep;
    int curr_row;
    int curr_epoch = 0;
    int i = 0;

    double dcost;
    double learning_rate = lr;
    double MSE;
    double pred;
    double wts_cng_fac[column_size] = {0};

    for (int i = 0; i < column_size; i++)
        weights[i] = -1;

    while (curr_epoch != epochs)
    {
        MSE = 0;
        curr_row = 0;
        while (curr_row < row_size)
        {
            for (int curr_batch_row = 0; (curr_batch_row < batch_size && curr_row < row_size); curr_batch_row++, curr_row++)
            {
                // h(x)
                dcost = weights[0] * 1;
                for (int i = 0; i <= column_size - 2; i++)
                {
                    dcost += (weights[i + 1] * data[curr_row][i]);
                }

                // (h(x) - y)
                dcost = (double)(dcost - data[curr_row][column_size - 1]);

                //(h(x) - y)x
                wts_cng_fac[0] += dcost;
                for (int i = 0; i <= column_size - 2; i++)
                {
                    wts_cng_fac[i + 1] += dcost * data[curr_row][i];
                }

                MSE += dcost * dcost;
            }

            wts_cng_fac[0] = (wts_cng_fac[0] * learning_rate) / (double)batch_size;
            for (int i = 0; i <= column_size - 2; i++)
            {
                wts_cng_fac[i + 1] = (wts_cng_fac[i + 1] * learning_rate) / (double)batch_size;
            }

            weights[0] = weights[0] - wts_cng_fac[0];
            for (int i = 0; i <= column_size - 2; i++)
            {
                weights[i + 1] = weights[i + 1] - wts_cng_fac[i + 1];
            }

            wts_cng_fac[0] = 0;
            for (int i = 0; i <= column_size - 2; i++)
            {
                wts_cng_fac[i + 1] = 0;
            }
        }

        //cout << "Epoch " << curr_epoch + 1 << " : " << MSE / ((double)row_size / (double)batch_size) << endl;
        curr_epoch++;
    }
}
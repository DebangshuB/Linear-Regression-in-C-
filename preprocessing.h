#include <iostream>
#include <fstream>
#include <iomanip>
#define precision 7

using namespace std;

//-------------------------Internal----------------------------

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

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

//-------------------------External----------------------------

int get_dimensions(char input_file[], int dimen[])
{
    ifstream fin(input_file);

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

int place_tuples(char inp[], double **tuples, int rows, int cols)
{
    ifstream fin(inp);

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
                tuples[r][c++] = curr;

                if (c >= cols)
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

void shuffle(double **tuples, int rows, int cols)
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
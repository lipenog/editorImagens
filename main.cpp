#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

/**
 * @brief Get the Rows and Col from a file *.pgm, does not suport files with comments
 *
 * @param nameFile name of the file
 * @param rows pointer to store the value
 * @param col pointer to store the value
 * @param maxValue pointer to store the value
 */
void getRowsCol(string nameFile, int *rows, int *col, int *maxValue)
{
    ifstream myFile(nameFile);
    string myLine;

    if (myFile.is_open())
    {
        cout << "FILE IS OPEN" << endl;
        // get out of the "P2"
        myFile >> myLine;
        myFile >> *col;
        myFile >> *rows;
        myFile >> *maxValue;
    }

    myFile.close();
}

/**
 * @brief read a file to get a image, need to use the getRowsCol before!
 *
 * @param nameFile name of the file
 * @param rows rows of the image
 * @param col col of the image
 *
 * @return a pointer to the "matrix", stored in format of a vector
 */
int *readImg(string nameFile, int rows, int col, int *img)
{

    ifstream myFile(nameFile);
    string myLine;
    int aux;

    if (myFile.is_open())
    {
        cout << "FILE IS OPEN" << endl;
        myFile >> myLine;
        myFile >> aux;
        myFile >> aux;
        myFile >> aux;

        for (int i = 0, *tmp = img; i < rows * col; i++, tmp++)
        {
            myFile >> *tmp;
        }
    }

    myFile.close();

    return img;
}

/**
 * @brief write a file with a image *.pgm
 *
 * @param nameFile name of the file
 * @param rows rows of the image
 * @param col col of the image
 * @param img pointer to the image
 */
void writeImg(string nameFile, int rows, int col, int maxValue, int *img)
{
    ofstream myFile(nameFile);
    string myLine = "P2";

    if (myFile.is_open())
    {
        cout << "FILE IS OPEN" << endl;
        myFile << myLine << endl;
        myFile << col << " " << rows << endl;
        myFile << maxValue << endl;
        for (int i = 0, *tmp = img; i < rows * col; i++, tmp++)
        {
            myFile << *tmp << " ";
            if (i % 17 == 0)
            {
                myFile << endl;
            }
        }
    }

    myFile.close();
}

void negative(int rows, int col, int *img, int *imgResult)
{
    for (int i = 0; i < rows * col; i++)
    {
        *imgResult = 255 - *img;
        imgResult++;
        img++;
    }
}

void levelOfLight(int rows, int col, int maxValue, int *img, int *imgResult, float levelOfLight)
{
    for (int i = 0; i < rows * col; i++)
    {
        *imgResult = *img * levelOfLight;

        if (*imgResult > maxValue)
        {
            *imgResult = maxValue;
        }

        if (*imgResult < 0)
        {
            *imgResult = 0;
        }

        imgResult++;
        img++;
    }
}

void transpose(int rows, int col, int *img, int *imgResult)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = i; j < col; j++)
        {
            imgResult[col * i + j] = img[col * j + i];
            imgResult[col * j + i] = img[col * i + j];
        }
    }
}

void horizontalRotation(int rows, int col, int *img, int *imgResult)
{
    int *p1 = imgResult;
    int *p2 = img + (col * rows) - col;

    for (int i = 0, j = 0; i < rows * col; i++, p1++, p2++, j++)
    {
        *p1 = *p2;
        if (j == col)
        {
            p2 = p2 - 2 * col;
            j = 0;
        }
    }
}

void verticalRotation(int rows, int col, int *img, int *imgResult)
{
    int *p1 = imgResult;
    int *p2 = img + col - 1;

    for (int i = 0, j = 0; i < rows * col; i++, p1 += col, p2 += col, j++)
    {
        *p1 = *p2;
        if (j == rows)
        {
            p1 = p1 - rows * col + col + 1;
            p2 = p2 - rows * col + 2;
            j = 0;
        }
    }
}

void rightRotation()
{
}

void leftRotation()
{
}

int main(void)
{
    int rows, col, maxValue;
    getRowsCol("mulher.pgm", &rows, &col, &maxValue);

    int *img = new int[rows * col];
    int *imgResult = new int[rows * col];

    readImg("mulher.pgm", rows, col, img);

    writeImg("teste.pgm", rows, col, maxValue, img);

    negative(rows, col, img, imgResult);
    writeImg("negative.pgm", rows, col, maxValue, imgResult);

    levelOfLight(rows, col, maxValue, img, imgResult, 1.8);
    writeImg("brighten.pgm", rows, col, maxValue, imgResult);

    levelOfLight(rows, col, maxValue, img, imgResult, 0.2);
    writeImg("darken.pgm", rows, col, maxValue, imgResult);

    transpose(rows, col, img, imgResult);
    writeImg("transpose.pgm", col, rows, maxValue, imgResult);

    horizontalRotation(rows, col, img, imgResult);
    writeImg("horizontal.pgm", rows, col, maxValue, imgResult);

    verticalRotation(rows, col, img, imgResult);
    writeImg("vertical.pgm", rows, col, maxValue, imgResult);

    return 0;
}

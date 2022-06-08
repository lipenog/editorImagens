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

int *negative(int rows, int col, int *img)
{
    int *imgResult = new int[col*rows];
    int *result = imgResult;

    for (int i = 0; i < rows * col; i++)
    {
        *imgResult = 255 - *img;
        imgResult++;
        img++;
    }

    return result;
}

int *levelOfLight(int rows, int col, int maxValue, int *img, float levelOfLight)
{
    int *imgResult = new int[col*rows];
    int *result = imgResult;

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

    return result;
}

int *transpose(int rows, int col, int *img)
{
    int *imgResult = new int[col*rows];

    int *p1 = imgResult;
    int *p2 = img;
    int *q1, *q2;
    for (; p1 < imgResult + (col*rows) - rows; p1 += rows, p2++)
    {
        for (q1 = p1, q2 = p2; q1 <= p1 + rows; q1++, q2 += col)
        {
            *q1 = *q2;
        }
    }

    return imgResult;
}

int *horizontalRotation(int rows, int col, int *img)
{
    int *imgResult = new int[col*rows];

    int *p1 = imgResult;
    int *p2 = img + (col * rows) - col;
    int *q1, *q2;

    for (; p1 <= imgResult + (col * rows) - col; p1 += col, p2 -= col)
    {
        for (q1 = p1, q2 = p2; q1 <= p1 + col; q1++, q2++)
        {
            *q1 = *q2;
        }
    }

    return imgResult;
}

int *verticalRotation(int rows, int col, int *img)
{
    int *imgResult = new int[col*rows];

    int *p1 = imgResult;
    int *p2 = img + col - 1;
    int *q1, *q2;

    for (; p1 <= imgResult + col - 1; p1++, p2--)
    {
        for (q1 = p1, q2 = p2; q1 <= p1 + (col * rows) - col; q1 += col, q2 += col)
        {
            *q1 = *q2;
        }
    }

    return imgResult;
}

int *rightRotation(int rows, int col, int *img)
{
    int *result = new int[rows*col];
    result = verticalRotation(col, rows, transpose(rows, col, img));
    return result;
}

int *leftRotation(int rows, int col, int *img)
{
    int *result = new int[rows*col];
    result = horizontalRotation(col, rows, transpose(rows, col, img));
    return result;
}

int *saltAndPepper(int rows, int col, int maxValue, int *img)
{
    int *imgResult = new int[rows*col];
    for (int *p1 = imgResult; p1 <= imgResult + (col*rows); p1++, img++)
    {
        int v1 = rand() % 100;
        if (v1 == 0)
        {
            *p1 = 0;
        } else if (v1 == 10) {
            *p1 = maxValue;
        } else {
            *p1 = *img;
        }
    }
    return imgResult;
}

int *padding(int rows, int col, int maxValue, int *img, int dimension)
{
    int colResult = (col+2*dimension), rowsResult = (rows+2*dimension); 
    int *imgResult = new int[colResult*rowsResult];
    int *p1, *p2, *q1, *q2;
    for (p1 = imgResult; p1 < imgResult + (colResult*rowsResult); p1++)
    {
        *p1 = maxValue;
    } 
  
    for (p1 = imgResult + (dimension*colResult) + dimension, p2 = img; p1 < imgResult + (dimension*colResult) + dimension + col; p1++, p2++)
    {
        for (q1 = p1, q2 = p2; q1 < p1 + (colResult*rowsResult) - 2*dimension*colResult; q1 += colResult, q2 += col)
        {
            *q1 = *q2;
        }
    }

    return imgResult;
}

int main(void)
{
    int rows, col, maxValue;
    getRowsCol("totem.pgm", &rows, &col, &maxValue);
    cout << rows << " " << col << endl;
    int *img = new int[rows * col];
    int *imgResult = new int[rows * col];
    int *imgPadding = new int[(col+2)*(rows+2)];
    int *imgPadding2 = new int[(col+4)*(rows+4)];
    int *imgPadding3 = new int[(col+6)*(rows+6)];

    readImg("totem.pgm", rows, col, img);

    imgResult = negative(rows, col, img);
    writeImg("negative.pgm", rows, col, maxValue, imgResult);

    imgResult = levelOfLight(rows, col, maxValue, img, 1.8);
    writeImg("brighten.pgm", rows, col, maxValue, imgResult);

    imgResult = levelOfLight(rows, col, maxValue, img, 0.2);
    writeImg("darken.pgm", rows, col, maxValue, imgResult);

    imgResult = transpose(rows, col, img);
    writeImg("transpose.pgm", col, rows, maxValue, imgResult);

    imgResult = horizontalRotation(rows, col, img);
    writeImg("horizontal.pgm", rows, col, maxValue, imgResult);

    imgResult = verticalRotation(rows, col, img);
    writeImg("vertical.pgm", rows, col, maxValue, imgResult);

    imgResult = rightRotation(rows, col, img);
    writeImg("rightR.pgm", col, rows, maxValue, imgResult);

    imgResult = leftRotation(rows, col, img);
    writeImg("leftR.pgm", col, rows, maxValue, imgResult);

    imgResult = saltAndPepper(rows, col, maxValue, img);
    writeImg("salt&pepper.pgm", rows, col, maxValue, imgResult);

    imgPadding = padding(rows, col, maxValue, img, 1);
    writeImg("padding.pgm", rows+2, col+2, maxValue, imgPadding);

    imgPadding2 = padding(rows, col, maxValue, img, 2);
    writeImg("padding2.pgm", rows+4, col+4, maxValue, imgPadding2);

    imgPadding3 = padding(rows, col, maxValue, img, 3);
    writeImg("padding3.pgm", rows+6, col+6, maxValue, imgPadding3);

    return 0;
}

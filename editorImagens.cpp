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
    for (int *p1 = imgResult; p1 < imgResult + (col*rows); p1++, img++)
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

int *padding(int rows, int col, int maxValue, int *img, int dimension, int *rows_result, int *col_result)
{
    int colResult = (col+2*dimension), rowsResult = (rows+2*dimension); 
    int *imgResult = new int[colResult*rowsResult];
    int *p1, *p2, *q1, *q2;
    *rows_result = rows + 2*dimension;
    *col_result = col + 2*dimension;
    for (p1 = imgResult; p1 < imgResult + (colResult*rowsResult); p1++)
    {
        *p1 = 0;
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

int *selectKernel(int rows, int col, int *img, int dimension, int *center, int len)
{
    int *result = new int[len];
    int *aux = result;
    int *leftElem = center - dimension - col;
    int *rightElem = center + dimension - col;
    for (int *p1 = leftElem, *p2 = rightElem; p1 <= leftElem + (dimension+1)*col; p1 += col, p2 += col)
    {
        for (int *q1 = p1, *q2 = p2; q1 <= q2; q1++, aux++)
        {
            *aux = *q1;
        }
    }
    return result;
}

void bubbleSort(int *a, int len)
{
    int aux;
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if (a[j] < a[j+1])
            {
                aux = a[j+1];
                a[j+1] = a[j];
                a[j] = aux;
            }
        }
    }
}

int *blur(int rows, int col, int maxValue, int *img, int dimension, int *rowsResult, int *colResult)
{
    int *result = new int[rows*col];
    int *imgResult = result;
    int *imgPadding = padding(rows, col, maxValue, img, dimension, rowsResult, colResult);
    float pixelValue;
    int kernelLen = (2*dimension+1)*(2*dimension+1);
    int *kernel = new int[kernelLen];

    int *leftUpPoint = imgPadding + (dimension**colResult) + dimension;
    int *leftDownPoint = (imgPadding + (*rowsResult**colResult) - *colResult) - dimension**colResult + dimension;


    for (int *p1 = leftUpPoint, *p2 = leftDownPoint; p1 != p2; p1 += *colResult)
    {
        for (int *q1 = p1; q1 < p1 + col; q1++)
        {
            pixelValue = 0;
            kernel = selectKernel(*rowsResult, *colResult, imgPadding, dimension, q1, kernelLen);
            for (int *aux = kernel; aux < kernel + kernelLen; aux++)
            {
                pixelValue += *aux;
            }
            *result = pixelValue/kernelLen;
            result++;
        }
    }

    return imgResult;
}

int *solveSaltAndPepper(int rows, int col, int maxValue, int *img, int dimension, int *rowsResult, int *colResult)
{
    int *result = new int[rows*col];
    int *imgResult = result;
    int *imgPadding = padding(rows, col, maxValue, img, dimension, rowsResult, colResult);
    int kernelLen = (2*dimension+1)*(2*dimension+1);
    int *kernel = new int[kernelLen];

    int *leftUpPoint = imgPadding + (dimension**colResult) + dimension;
    int *leftDownPoint = (imgPadding + (*rowsResult**colResult) - *colResult) - dimension**colResult + dimension;


    for (int *p1 = leftUpPoint, *p2 = leftDownPoint; p1 != p2; p1 += *colResult)
    {
        for (int *q1 = p1; q1 < p1 + col; q1++)
        {
            kernel = selectKernel(*rowsResult, *colResult, imgPadding, dimension, q1, kernelLen);
            bubbleSort(kernel, kernelLen);
            *result = kernel[kernelLen/2];
            result++;
        }
    }

    return imgResult;
}

int main(void)
{
    int rows, col, maxValue;
    int rows_result, col_result;
    getRowsCol("mulher.pgm", &rows, &col, &maxValue);
    cout << rows << " " << col << endl;
    int *img = new int[rows * col];
    int *imgResult = new int[rows * col];
    int *imgPadding;
    readImg("mulher.pgm", rows, col, img);
    
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
    
    imgPadding = padding(rows, col, maxValue, img, 2, &rows_result, &col_result);
    writeImg("padding.pgm", rows_result, col_result, maxValue, imgPadding);

    imgPadding = blur(rows, col, maxValue, img, 1, &rows_result, &col_result);
    writeImg("blur3x3.pgm", rows, col, maxValue, imgPadding);

    imgPadding = blur(rows, col, maxValue, img, 2, &rows_result, &col_result);
    writeImg("blur5x5.pgm", rows, col, maxValue, imgPadding);

    imgPadding = blur(rows, col, maxValue, img, 3, &rows_result, &col_result);
    writeImg("blur9x9.pgm", rows, col, maxValue, imgPadding);

    imgPadding = blur(rows, col, maxValue, img, 20, &rows_result, &col_result);
    writeImg("blur20.pgm", rows, col, maxValue, imgPadding);
    
    imgResult = saltAndPepper(rows, col, maxValue, img);
    writeImg("salt&pepper.pgm", rows, col, maxValue, imgResult);

    imgPadding = solveSaltAndPepper(rows, col, maxValue, imgResult, 1, &rows_result, &col_result);
    writeImg("solveSalt&Pepper.pgm", rows, col, maxValue, imgPadding);

    imgPadding = solveSaltAndPepper(rows, col, maxValue, imgResult, 2, &rows_result, &col_result);
    writeImg("solveSalt&Pepper2.pgm", rows, col, maxValue, imgPadding);
    // 10:34
    //imgPadding = solveSaltAndPepper(rows, col, maxValue, imgResult, 10, &rows_result, &col_result);
    //writeImg("solveSalt&Pepper3.pgm", rows, col, maxValue, imgPadding);
    // 10:36

    //10:37
    imgPadding = solveSaltAndPepper(rows, col, maxValue, imgResult, 20, &rows_result, &col_result);
    writeImg("solveSalt&Pepper4.pgm", rows, col, maxValue, imgPadding);
    return 0;
}

#pragma once
#include <iostream>
#include <raylib.h>

//the name means matrix 

using namespace std;

enum error {//errors if the operation is invalid
	ADDITION_ERROR,
	MULTIPLICATION_ERROR,
	DET_ERROR,
	NONE
};

Vector3 rotatePointAroundAxis(Vector3 point, Vector3 axis, float angle);

class matrice
{
public:
	int n, m;// m/i nr linii, n/j nr coloane
	double values[3][3] = { 0 };
	error operationError = NONE;

	matrice(int linii = 3, int coloane = 3)//initializer
	{
		n = coloane;
		m = linii;
	}

	void show()//just for debug purposes
	{
		cout << m << ' ' << n << '\n';
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << values[i][j] << ' ';
			}
			cout << '\n';
		}
		cout << '\n';
	}

	double det()//determinant, it is hard coded for 1x1, 2x2 and 3x3 matrices
	{
		if (m != n)
		{
			cout << "NOT POSSIBLE\n";
			operationError = DET_ERROR;
			return 0;
		}
		operationError = NONE;
		if (m == 1)
		{
			return values[0][0];
		}
		else if (m == 2)
		{
			return values[0][0] * values[1][1] - values[0][1] * values[1][0];
		}
		else
		{
			return values[0][0] * values[1][1] * values[2][2] + values[1][0] * values[2][1] * values[0][2] + values[2][0] * values[0][1] * values[1][2] - values[0][2] * values[1][1] * values[2][0] - values[0][0] * values[1][2] * values[2][1] - values[0][1] * values[1][0] * values[2][2];
		}
	}

	bool operator==(const matrice& other)//checks equality
	{
		if (n != other.n || m != other.m)
		{
			return false;
		}
		for (int i = 0; i < m; i++) // linii
		{
			for (int j = 0; j < n; j++) //coloane
			{
				if (values[i][j] != other.values[i][j])
					return false;
			}
		}
		return true;
	}

	matrice operator+(const matrice& other)//adds two matrices
	{
		if (n != other.n || m != other.m)
		{
			cout << "NOT POSSIBLE\n";
			operationError = ADDITION_ERROR;
			return 0;
		}
		operationError = NONE;
		matrice sum(m, n);
		for (int i = 0; i < m; i++) // linii
		{
			for (int j = 0; j < n; j++) //coloane
			{
				sum.values[i][j] = values[i][j] + other.values[i][j];
			}
		}
		return sum;
	}

	matrice operator-(const matrice& other)//substacts two matrices
	{
		if (n != other.n || m != other.m)
		{
			cout << "NOT POSSIBLE\n";
			operationError = ADDITION_ERROR;
			return 0;
		}
		operationError = NONE;
		matrice diff;
		for (int i = 0; i < m; i++) // linii
		{
			for (int j = 0; j < n; j++) //coloane
			{
				diff.values[i][j] = values[i][j] - other.values[i][j];
			}
		}
		return diff;
	}

	void operator=(const matrice& other)//assigning a value to a matix
	{
		n = other.n;
		m = other.m;
		for (int i = 0; i < m; i++)//linii
		{
			for (int j = 0; j < n; j++)//coloane
			{
				values[i][j] = other.values[i][j];
			}
		}
	}

	matrice operator*(const double& scalar)//multiplying with a scalar
	{
		matrice prod(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				prod.values[i][j] = values[i][j] * scalar;
			}
		}
		return prod;
	}

	matrice operator*(const matrice& other)//multiplying two matrices
	{
		if (n != other.m)
		{
			cout << "NOT POSSIBLE\n";
			operationError = MULTIPLICATION_ERROR;
		}
		operationError = NONE;
		matrice prod(m, other.n);

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < other.n; j++)
			{
				double val = 0;
				for (int x = 0; x < n; x++)
				{
					val += values[i][x] * other.values[x][j];
				}
				prod.values[i][j] = val;
			}
		}

		return prod;
	}
};
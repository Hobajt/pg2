#ifndef MATRIX_3X3_H_
#define MATRIX_3X3_H_

#include "vector3.h"
#include <iostream>

struct quat;

/*! \class Matrix3x3
\brief Re�ln� matice 3x3 uspo��dan� po ��dc�ch.

\author Tom� Fabi�n
\version 1.1
\date 2018
*/
class Matrix3x3 {
public:
	//! V�choz� konstruktor.
	/*!
	Inicializace na matici identity.
	*/
	Matrix3x3();

	//! V�choz� konstruktor.
	/*!
	Inicializace matice zadan�mi hodnotami. Prvn� index ozna�uje ��dek, druh� index pat�� sloupci.
	*/
	Matrix3x3(const float m00, const float m01, const float m02,
			  const float m10, const float m11, const float m12,
			  const float m20, const float m21, const float m22);

	Matrix3x3(const Vector3 basis_x, const Vector3 basis_y, const Vector3 basis_z);
	Matrix3x3(const quat& q);

	//! Transpozice matice.
	/*!
	Provede traspozici matice vz�jemnou v�m�nou ��dk� a sloupc�.
	*/
	Matrix3x3 Transpose() const;
	inline Matrix3x3 transpose() const { return transpose(); }

	//! Nastav� zadan� prvek matice na novou hodnotu.
	/*!
	\param row ��dek matice.
	\param column sloupec matice.
	\param value nov� hodnota prvku matice.
	*/
	void set(const int row, const int column, const float value);

	//! Vr�t� zadan� prvek matice.
	/*!
	\param row ��dek matice.
	\param column sloupec matice.
	\return Po�adovan� prvek matice.
	*/
	float get(const int row, const int column) const;

	float& operator()(int row, int col);
	const float& operator()(int row, int col) const;

	Vector3 column(const int column) const;
	Vector3 row(const int row) const;

	float* data();

	friend Vector3 operator*(const Matrix3x3& a, const Vector3& b);
	friend Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b);

	friend std::ostream& operator<<(std::ostream& os, const Matrix3x3& m);

	static Matrix3x3 EulerX(float angleRad);
	static Matrix3x3 EulerY(float angleRad);
	static Matrix3x3 EulerZ(float angleRad);
private:
#pragma warning( push )
#pragma warning ( disable : 4201 )
	union {
		/* Row-major
		m00 m01 m02
		m10 m11 m12
		m20 m21 m22
		*/

		struct {
			float m00_;
			float m01_;
			float m02_;

			float m10_;
			float m11_;
			float m12_;

			float m20_;
			float m21_;
			float m22_;
		};

		struct {
			Vector3 r_[3];
		};

		float data_[3 * 3];
	};
#pragma warning( pop ) 
};

typedef Matrix3x3 Matrix3;

using mat3f = Matrix3x3;

#endif

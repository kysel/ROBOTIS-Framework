/*
 * RobotisLinearAlgebra.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: jay
 */


#include "robotis_math/RobotisLinearAlgebra.h"

namespace ROBOTIS
{

Eigen::MatrixXd transitionXYZ ( double position_x, double position_y, double position_z )
{
    Eigen::MatrixXd _position(3,1);

    _position << position_x,
                 position_y,
                 position_z;

    return _position;
}

Eigen::MatrixXd transformationXYZRPY ( double position_x, double position_y, double position_z , double roll , double pitch , double yaw )
{
//    Eigen::MatrixXd _position(3,1);
//
//    _position << position_x,
//                 position_y,
//                 position_z;
//
//    Eigen::MatrixXd _rotation = rpy2rotation( roll , pitch , yaw );
//
//    Eigen::MatrixXd _transformation(4,4);
//
//    _transformation << _rotation , _position,
//                       0 , 0 , 0 , 1;

    Eigen::MatrixXd _transformation = rotation4d(roll, pitch, yaw);
    _transformation.coeffRef(0,3) = position_x;
    _transformation.coeffRef(1,3) = position_y;
    _transformation.coeffRef(2,3) = position_z;

    return _transformation;
}

Eigen::MatrixXd InverseTransformation(Eigen::MatrixXd transform)
{
	Eigen::Vector3d vecBOA; //If T is Transform Matrix A from B, the BOA is translation component coordi. B to coordi. A
	Eigen::Vector3d vec_x, vec_y, vec_z;
	Eigen::MatrixXd _invT(4,4);

	vecBOA.coeffRef(0) = -transform(0,3); vecBOA(1) = -transform(1,3); vecBOA(2) = -transform(2,3);
	vec_x(0) = transform(0,0); vec_x(1) = transform(1,0); vec_x(2) = transform(2,0);
	vec_y(0) = transform(0,1); vec_y(1) = transform(1,1); vec_y(2) = transform(2,1);
	vec_z(0) = transform(0,2); vec_z(1) = transform(1,2); vec_z(2) = transform(2,2);
//
//
//	// inv = [   x'   | -AtoB??x ]
//	//       [   y'   | -AtoB??y ]
//	//       [   z'   | -AtoB??z ]
//	//       [  0 0 0  |       1 ]
//
	_invT << vec_x(0), vec_x(1), vec_x(2), vecBOA.dot(vec_x),
			vec_y(0), vec_y(1), vec_y(2), vecBOA.dot(vec_y),
			vec_z(0), vec_z(1), vec_z(2), vecBOA.dot(vec_z),
			      0,        0,        0,                   1;

	return _invT;
}

Eigen::MatrixXd inertiaXYZ( double ixx, double ixy, double ixz , double iyy , double iyz, double izz )
{
    Eigen::MatrixXd _inertia(3,3);

    _inertia << ixx , ixy , ixz,
                ixy , iyy , iyz,
                ixz , iyz , izz ;

    return _inertia;
}

Eigen::MatrixXd rotationX( double angle )
{
    Eigen::MatrixXd _rotation( 3 , 3 );

    _rotation << 1.0,          0.0,           0.0,
                 0.0, cos( angle ), -sin( angle ),
                 0.0, sin( angle ),  cos( angle );

    return _rotation;
}

Eigen::MatrixXd rotationY( double angle )
{
    Eigen::MatrixXd _rotation( 3 , 3 );

    _rotation << cos( angle ), 0.0, sin( angle ),
                          0.0, 1.0, 	     0.0,
                -sin( angle ), 0.0, cos( angle );

    return _rotation;
}

Eigen::MatrixXd rotationZ( double angle )
{
    Eigen::MatrixXd _rotation(3,3);

    _rotation << cos( angle ), -sin( angle ), 0.0,
                 sin( angle ),  cos( angle ), 0.0,
                          0.0,           0.0, 1.0;

    return _rotation;
}

Eigen::MatrixXd rotation2rpy( Eigen::MatrixXd rotation )
{
    Eigen::MatrixXd _rpy = Eigen::MatrixXd::Zero( 3 , 1 );

    _rpy.coeffRef( 0 , 0 ) = atan2( rotation.coeff( 2 , 1 ), rotation.coeff( 2 , 2 ) );
    _rpy.coeffRef( 1 , 0 ) = atan2( -rotation.coeff( 2 , 0 ), sqrt( pow( rotation.coeff( 2 , 1 ) , 2 ) + pow( rotation.coeff( 2 , 2 ) , 2 ) ) );
    _rpy.coeffRef( 2 , 0 ) = atan2 ( rotation.coeff( 1 , 0 ) , rotation.coeff( 0 , 0 ) );

    return _rpy;
}

Eigen::MatrixXd rpy2rotation( double roll, double pitch, double yaw )
{
    Eigen::MatrixXd _rotation = rotationZ( yaw ) * rotationY( pitch ) * rotationX( roll );

    return _rotation;
}

Eigen::Quaterniond rpy2quaternion( double roll, double pitch, double yaw )
{
    Eigen::MatrixXd _rotation = rpy2rotation( roll, pitch, yaw );

    Eigen::Matrix3d _rotation3d;
    _rotation3d = _rotation.block<3,3>( 0 , 0);

    Eigen::Quaterniond _quaternion;

    _quaternion = _rotation3d;

    return _quaternion;
}

Eigen::Quaterniond rotation2quaternion( Eigen::MatrixXd rotation )
{
    Eigen::Matrix3d _rotation3d;

    _rotation3d = rotation.block<3,3>( 0 , 0);

    Eigen::Quaterniond _quaternion;
    _quaternion = _rotation3d;

    return _quaternion;
}

Eigen::MatrixXd quaternion2rpy( Eigen::Quaterniond quaternion )
{
    Eigen::MatrixXd _rpy = rotation2rpy( quaternion.toRotationMatrix() );

    return _rpy;
}

Eigen::MatrixXd quaternion2rotation( Eigen::Quaterniond quaternion )
{
    Eigen::MatrixXd _rotation = quaternion.toRotationMatrix();

    return _rotation;
}

Eigen::MatrixXd rotation4d( double roll, double pitch, double yaw )
{
//    Eigen::MatrixXd _rotation4d = Eigen::MatrixXd::Zero( 4 , 4 );
//    _rotation4d.coeffRef( 3 , 3 ) = 1.0;
//
//    Eigen::MatrixXd _rotation = rotationZ( yaw ) * rotationY( pitch ) * rotationX( roll );
//
////    _rotation4d.block<3,3>(0,0) = _rotation;
//    _rotation4d.coeffRef(0,0) = _rotation.coeff(0,0);
//    _rotation4d.coeffRef(0,1) = _rotation.coeff(0,1);
//    _rotation4d.coeffRef(0,2) = _rotation.coeff(0,2);
//    _rotation4d.coeffRef(1,0) = _rotation.coeff(1,0);
//    _rotation4d.coeffRef(1,1) = _rotation.coeff(1,1);
//    _rotation4d.coeffRef(1,2) = _rotation.coeff(1,2);
//    _rotation4d.coeffRef(2,0) = _rotation.coeff(2,0);
//    _rotation4d.coeffRef(2,1) = _rotation.coeff(2,1);
//    _rotation4d.coeffRef(2,2) = _rotation.coeff(2,2);

//	return _rotation4d;
	double sr = sin(roll), cr = cos(roll);
	double sp = sin(pitch), cp = cos(pitch);
	double sy = sin(yaw), cy = cos(yaw);

	Eigen::MatrixXd matRoll(4,4);
	Eigen::MatrixXd matPitch(4,4);
	Eigen::MatrixXd matYaw(4,4);

	matRoll << 1,   0,   0,  0,
			0,  cr, -sr,  0,
			0,  sr,  cr,  0,
			0,   0,   0,  1;

	matPitch << cp,  0,  sp,  0,
				 0,  1,   0,  0,
			   -sp,  0,  cp,  0,
				 0,  0,   0,  1;

	matYaw << cy, -sy,  0,  0,
			  sy,  cy,  0,  0,
			   0,   0,  1,  0,
			   0,   0,  0,  1;


	return (matYaw*matPitch)*matRoll;
}



Eigen::MatrixXd hatto( Eigen::MatrixXd matrix3d )
{
    Eigen::MatrixXd _hatto(3,3);

    _hatto <<  0.0, 		        -matrix3d.coeff(2,0),  matrix3d.coeff(1,0),
               matrix3d.coeff(2,0),                  0.0, -matrix3d.coeff(0,0),
              -matrix3d.coeff(1,0),  matrix3d.coeff(0,0),                  0.0;

    return _hatto;
}

Eigen::MatrixXd Rodrigues( Eigen::MatrixXd hat_matrix , double angle )
{
    Eigen::MatrixXd _E	 = 	Eigen::MatrixXd::Identity( 3 , 3 );
    Eigen::MatrixXd _Rodrigues = _E + hat_matrix * sin( angle ) + hat_matrix * hat_matrix * ( 1 - cos( angle ) );

    return _Rodrigues;
}

Eigen::MatrixXd rot2omega( Eigen::MatrixXd rotation )
{
    double _eps = 1e-12;

//    Eigen::MatrixXd _E = Eigen::MatrixXd::Identity( 3 , 3 );

    double _alpha = ( rotation.coeff( 0 , 0 ) + rotation.coeff( 1 , 1 ) + rotation.coeff( 2 , 2 ) - 1.0 ) / 2.0 ;

    double _alpha_plus = fabs( _alpha - 1.0 );

    Eigen::MatrixXd _rot2omega( 3 , 1 );

    if( _alpha_plus < _eps )
    {
        _rot2omega << 0.0,
                      0.0,
                      0.0;
    }
    else
    {
        double _theta = acos( _alpha );

        _rot2omega << rotation.coeff( 2 , 1 ) - rotation.coeff( 1 , 2 ),
                      rotation.coeff( 0 , 2 ) - rotation.coeff( 2 , 0 ),
                      rotation.coeff( 1 , 0 ) - rotation.coeff( 0 , 1 );

        _rot2omega = 0.5 * ( _theta / sin( _theta ) ) * _rot2omega;
    }

    return _rot2omega;
}

Eigen::MatrixXd cross(Eigen::MatrixXd vector3d_a, Eigen::MatrixXd vector3d_b )
{
    Eigen::MatrixXd _cross( 3 , 1 );

    _cross << vector3d_a.coeff( 1 , 0 ) * vector3d_b.coeff( 2 , 0 ) - vector3d_a.coeff( 2 , 0 ) * vector3d_b.coeff( 1 , 0 ),
    		  vector3d_a.coeff( 2 , 0 ) * vector3d_b.coeff( 0 , 0 ) - vector3d_a.coeff( 0 , 0 ) * vector3d_b.coeff( 2 , 0 ),
			  vector3d_a.coeff( 0 , 0 ) * vector3d_b.coeff( 1 , 0 ) - vector3d_a.coeff( 1 , 0 ) * vector3d_b.coeff( 0 , 0 );

    return _cross;
}

double dot(Eigen::MatrixXd vector3d_a, Eigen::MatrixXd vector3d_b )
{
	return vector3d_a.dot(vector3d_b);
	//return vector3d_a.coeff(0,0)*vector3d_b.coeff(0,0) + vector3d_a.coeff(1,0)*vector3d_b.coeff(1,0) + vector3d_a.coeff(2,0)*vector3d_b.coeff(2,0);
}

}

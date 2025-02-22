 /*
 * chassisMove.hpp
 *
 *  Created on: Jan 02, 2025
 *      Author: @sofiaariasv2002
 *              @AnaValeria
 * 
 * For mor information:
 * https://learning.oreilly.com/library/view/wheeled-mobile-robotics/9780128042380/B9780128042045000020_1.xhtml#s0070
 * https://www.robomaster.com/en-US/products/components/general/M3508
 *
 */

 #include <Eigen/Dense> 
 #include "IntfMotor.hpp" 
 #include "ControllerCAN.hpp"
 
 #ifndef CHASSIS_MOVE_HPP
 #define CHASSIS_MOVE_HPP
 #define CHASSIS_RADIUS 0.3f  // Radio del chasis (distancia del centro a una rueda) en metros
 #define MAX_MOTOR_SPEED 465.0f // Velocidad máxima del motor rpm
 #define K_TWIST 1.0f         // Sensibilidad para torsión del chasis
 #define PI 3.14159265358979323846
 
 float theta_robot_rads; //angulo actual del robot
 
 /**
  * @brief Clase para controlar el movimiento de un chasis mecanum utilizando joysticks.
  * 
  * Esta clase permite controlar el movimiento de un robot con chasis mecanum. La clase proporciona 
  * métodos para convertir las entradas de joystick en velocidades de motor, además de normalizar 
  * las velocidades y calcular la torsión en base a las entradas de control.
  */
 class chassisMove {
 private:
     IntfMotor* leftFrontMotor;
     IntfMotor* rightFrontMotor;
     IntfMotor* leftBackMotor;
     IntfMotor* rightBackMotor;
 
     float maxMotorSpeed_rpm; 
 
     float normalizeSpeed(float speed);
     float normalizeW(float theta_joy_rads);
 
 public:
     chassisMove(IntfMotor* leftFrontMotor, IntfMotor* rightFrontMotor,
                 IntfMotor* leftBackMotor, IntfMotor* rightBackMotor, 
                 float maxMotorSpeed_rpm = MAX_MOTOR_SPEED);
 
     void joystickToMotors(float x1, float y1, float x2, float y2);
 
     void stop();
 };
 
 #endif // CHASSIS_MOVE_HPP
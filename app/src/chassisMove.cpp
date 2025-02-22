#include "chassisMove.hpp"

/**
 * @brief Normaliza la velocidad del motor para asegurarse de que no exceda la velocidad máxima.
 * 
 * @param speed Velocidad a normalizar.
 * @return La velocidad normalizada.
*/
float chassisMove::normalizeSpeed(float speed) {
    if (speed > maxMotorSpeed_rpm) return maxMotorSpeed_rpm;
    if (speed < -maxMotorSpeed_rpm) return -maxMotorSpeed_rpm;
    return speed;
}

/**
 * @brief Normaliza la velocidad angular (torsión) en función de los valores de entrada del joystick.
 * 
 * @param theta_joy Ángulo del joystick que indica la dirección de la torsión.
 * @param theta_robot Ángulo actual del robot.
 * @return La velocidad angular normalizada en radianes por segundo.
 */
float chassisMove::normalizeW(float theta_joy_rads) {
    float angle_error = theta_joy_rads - theta_robot_rads;
    if (angle_error > M_PI) angle_error -= 2 * PI;
    if (angle_error < -M_PI) angle_error += 2 * PI;
    float w_rs = K_TWIST * angle_error;
    //TODO: si se actualiza?
    //theta_robot_rads=w_rs; 
    return w_rs;
}

chassisMove::chassisMove(IntfMotor* leftFrontMotor, IntfMotor* rightFrontMotor,
                         IntfMotor* leftBackMotor, IntfMotor* rightBackMotor, 
                         float maxMotorSpeed)
    : leftFrontMotor(leftFrontMotor), rightFrontMotor(rightFrontMotor),
      leftBackMotor(leftBackMotor), rightBackMotor(rightBackMotor),
      maxMotorSpeed(maxMotorSpeed_rpm) {}

/**
 * @brief Convierte las entradas de los joysticks en velocidades de los motores.
 * 
 * Este método toma las entradas de los dos joysticks (para movimiento y torsión) y las convierte en 
 * velocidades para cada rueda del chasis mecanum.
 * 
 * @param x1 Entrada del joystick 1 (eje X para desplazamiento en el plano horizontal).
 * @param y1 Entrada del joystick 1 (eje Y para desplazamiento en el plano vertical).
 * @param x2 Entrada del joystick 2 (eje X para control de torsión).
 * @param y2 Entrada del joystick 2 (eje Y para control de torsión).
 * @param theta_robot Ángulo de orientación del robot (en radianes).
 * @param theta_joy Ángulo de orientación del joystick2 (en radianes).
 */
void chassisMove::joystickToMotors(float x1, float y1, float x2, float y2) {
    // Cálculo del ángulo deseado 
    float theta_joy_rads = atan2(y2, x2);
    // Cálculo de la torsión (velocidad angular)
    float w_rs = normalizeW(theta_joy_rads)

    // u
    Eigen::Vector3f joystick_input(x1, y1, w);
    // M
    Eigen::MatrixXf control_matrix(4, 3);
    control_matrix << -1, -1,  CHASSIS_RADIUS,  // Delantera izquierda
                       1, -1,  CHASSIS_RADIUS,  // Delantera derecha
                       1,  1,  CHASSIS_RADIUS,  // Trasera derecha
                      -1,  1,  CHASSIS_RADIUS;  // Trasera izquierda

    //v=M*u
    Eigen::VectorXf wheel_speed = control_matrix * joystick_input;
    wheel_speed = wheel_speed.unaryExpr([this](float speed) { return normalizeSpeed(speed); });

    leftFrontMotor->actuate(wheel_speed[0]);   // Delantera izquierda
    rightFrontMotor->actuate(wheel_speed[1]);  // Delantera derecha
    rightBackMotor->actuate(wheel_speed[2]);   // Trasera derecha
    leftBackMotor->actuate(wheel_speed[3]);    // Trasera izquierda   
}

void chassisMove::stop() {
    leftFrontMotor->stop(0);
    rightFrontMotor->stop(0);
    leftBackMotor->stop(0);
    rightBackMotor->stop(0);
}
 //PARTE ANA - leer referencia de velovidad en el IMU
 //x1, y1, z1 son aceleromentro 
 //x2, y2, z2 son giroscopio 

//se llaman los valores dados por el acelerometro de la imu y por las velocidades de los motores.
void chassisMove::LecturaDeDatos(float x1, float y1,float z1 ,float x2,float y2,float z2 ){

     // Leer los datos del acelerómetro
    int16_t ax = IMU_ReadRegister16(0x3B);  // Eje X https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
    int16_t ay = IMU_ReadRegister16(0x3D);  // Eje Y  se encuentra en la pagina 7 del datasheet
    int16_t az = IMU_ReadRegister16(0x3F);  // Eje Z

    // Leer los datos del giroscopio
    int16_t gx = IMU_ReadRegister16(0x43);  // Eje X
    int16_t gy = IMU_ReadRegister16(0x45);  // Eje Y
    int16_t gz = IMU_ReadRegister16(0x47);  // Eje Z

    float x1g = x1 * (9.81/16384.0);  // Conversión dependiendo del acelerometro es el de 16384 y el 9.81 es valor gravedad nos da asi la aceleracion en m/s^2
    float y1g = y1 * (9.81/16384.0);    //https://www.diarioelectronicohoy.com/blog/configurar-el-mpu6050 ----se que tal vez no es muy confiable pero es entendible 
    float z1g = z1 * (9.81/16384.0);
    float x2g = x2 * (250.0/32767.0);
    float y2g = y2 * (250.0/32767.0);
    float z2g = z2 * (250.0/32767.0); // Conversión dependiendo del giroscopio es el de 32767  valores y el 250 grados por segundo es valor gravedad nos da en grados por segundo 

    //para imprimir los valores dados por el acelerometro de la imu y por las velocidades de los motores.
   printf("Acelerómetro: X=%.2f , Y=%.2f , Z=%.2f \n", x 1g, y1g, z1g);
   printf("giroscopio: X=%.2f , Y=%.2f , Z=%.2f \n", x2g, y2g, z2g);
   //printf("velocidades llantas: LF=%.2f , RF=%.2f , RB=%.2f, LB=%.2f \n", leftFrontSpeed, rightFrontSpeed, rightBackSpeed, leftBackSpeed);

}

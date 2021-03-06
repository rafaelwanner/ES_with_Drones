#include "flight_control.h"
#include "rc.h"
#include <math.h>

float pid_x_integ1 = 0;
float pid_y_integ1 = 0;
float pid_z_integ1 = 0;
float pid_x_integ2 = 0;
float pid_y_integ2 = 0;
float pid_z_integ2 = 0;
float pid_x_pre_error2 = 0;
float pid_y_pre_error2 = 0;
float pid_z_pre_error2 = 0;
float pid_x_pre_deriv = 0;
float pid_y_pre_deriv = 0;

extern int16_t gTHR;
int16_t motor_thr;
float dt_recip;

void PIDControlInit(P_PI_PIDControlTypeDef *pid)
{
  pid->ts = PID_SAMPLING_TIME;

  pid->x_kp1 = PITCH_PID_KP1;
  pid->x_ki1 = PITCH_PID_KI1;
  pid->x_i1_limit = PITCH_PID_I1_LIMIT;
  pid->x_kp2 = PITCH_PID_KP2;
  pid->x_ki2 = PITCH_PID_KI2;
  pid->x_kd2 = PITCH_PID_KD2;
  pid->x_i2_limit = PITCH_PID_I2_LIMIT;
  pid->x_s1 = 0;
  pid->x_s2 = 0;

  pid->y_kp1 = ROLL_PID_KP1;
  pid->y_ki1 = ROLL_PID_KI1;
  pid->y_i1_limit = ROLL_PID_I1_LIMIT;
  pid->y_kp2 = ROLL_PID_KP2;
  pid->y_ki2 = ROLL_PID_KI2;
  pid->y_kd2 = ROLL_PID_KD2;
  pid->y_i2_limit = ROLL_PID_I2_LIMIT;
  pid->y_s1 = 0;
  pid->y_s2 = 0;

  pid->z_kp1 = YAW_PID_KP1;
  pid->z_ki1 = YAW_PID_KI1;
  pid->z_i1_limit = YAW_PID_I1_LIMIT;
  pid->z_kp2 = YAW_PID_KP2;
  pid->z_ki2 = YAW_PID_KI2;
  pid->z_kd2 = YAW_PID_KD2;
  pid->z_i2_limit = YAW_PID_I2_LIMIT;
  pid->z_s1 = 0;
  pid->z_s2 = 0;
}


void FlightControlPID_OuterLoop(EulerAngleTypeDef *euler_rc, EulerAngleTypeDef *euler_ahrs, AHRS_State_TypeDef *ahrs, P_PI_PIDControlTypeDef *pid)
{
  float error;

  if(gTHR<MIN_THR)
  {
    pid_x_integ1 = 0;
    pid_y_integ1 = 0;
    pid_z_integ1 = 0;
  }

  /*
   * Roll PID:
   * Definition:
   * Input:
   * Roll angle value: euler_ahrs->thx
   * Roll angle setpoint (remote control): euler_rc->thx
   *
   * Intermediary variables:
   * Loop period (loop executed every pid->ts seconds): pid->ts
   * Integral value for Roll's PID: pid_x_integ1
   * Integral should be limited to the interval [-pid->x_i1_limit, pid->x_i1_limit]
   * PID constants: pid->x_kp1, pid->x_ki1
   * Important: For the outer loop, pid->y_kd1 = 0; i.e., this is a PI controller
   *
   * Controller output: pid->x_s1
   */

  // ****************************CODE_BEGIN****************************


  // *****************************CODE_END*****************************



  /*
   * Pitch PID:
   * Definition:
   * Input:
   * Pitch angle value: euler_ahrs->thy
   * Pitch angle setpoint (remote control): euler_rc->thy
   *
   * Intermediary variables:
   * Loop period (loop executed every pid->ts seconds): pid->ts
   * Integral value for Pitch's PID: pid_y_integ1
   * Integral should be limited to the interval [-pid->y_i1_limit, pid->y_i1_limit]
   * PID constants: pid->y_kp1, pid->y_ki1
   * Important: For the outer loop, pid->y_kd1 = 0; i.e., this is a PI controller
   *
   * Controller output: pid->y_s1
   */

  // ****************************CODE_BEGIN****************************


  // *****************************CODE_END*****************************



  /*
   * Yaw PID:
   * Definition:
   * Input:
   * Yaw angle value: euler_ahrs->thz
   * Yaw angle setpoint (remote control): euler_rc->thz
   *
   * Intermediary variables:
   * Loop period (loop executed every pid->ts seconds): pid->ts
   * Integral value for Yaw's PID: pid_z_integ1
   * Integral should be limited to the interval [-pid->z_i1_limit, pid->z_i1_limit]
   * PID constants: pid->z_kp1, pid->z_ki1
   * Important: For the outer loop, pid->z_kd1 = 0; i.e., this is a PI controller
   *
   * Controller output: pid->z_s1
   */

  // ****************************CODE_BEGIN****************************


  // *****************************CODE_END*****************************
}

void FlightControlPID_innerLoop(EulerAngleTypeDef *euler_rc, Gyro_Rad *gyro_rad, AHRS_State_TypeDef *ahrs, P_PI_PIDControlTypeDef *pid, MotorControlTypeDef *motor_pwm)
{
  float error, deriv;

  if(gTHR<MIN_THR)
  {
    pid_x_integ2 = 0;
    pid_y_integ2 = 0;
    pid_z_integ2 = 0;
  }
  
  dt_recip = 1/pid->ts;


  /*
   * Roll inner PID:
   * Definition:
   * Input:
   * Gyro value: gyro_rad->gx
   * Setpoint (from the outer PID): pid->x_s1
   *
   * Intermediary variables:
   * Loop period (loop executed every pid->ts seconds): pid->ts
   * Integral variable (store here the error's integral): pid_x_integ2
   * Integral should be limited to the interval [-pid->x_i2_limit, pid->x_i2_limit]
   * Derivative variable (store here the derivative): deriv
   * Store here the error's previous value: pid_x_pre_error2
   * Store here the derivative's previous value - after filtering: pid_x_pre_deriv
   * Filter's law: deriv = pid_x_pre_deriv + (deriv - pid_x_pre_deriv)*D_FILTER_COFF;
   *
   * PID constants: pid->x_kp2, pid->x_ki2, pid->y_kd2
   *
   * Controller output: pid->x_s2
   */

  // ****************************CODE_BEGIN****************************
  error = pid->x_s1 - gyro_rad->gx;

  if(pid->x_integ2 > pid->x_i2_limit){
	  pid->x_integ = pid->x_i2_limit;
  } else if(pid->x_integ2 < pid->x_i2_limit){
	  pid->x_integ = -pid->x_i2_limit;
  }

  deriv = (error - pid_x_pre_error2)*dt_recip;
  pid_x_pre_error2 = error;
  deriv = pid_x_pre_deriv + (deriv - pid_x_pre_deriv)*D_FILTER_COFF;
  pid_x_pre_deriv = deriv;

  pid->x_s2 = pid->x_kp2 * error + pid->x_ki2*pid_x_integ2 + pid->x_kd2*deriv;

  if(pid->x_s2 > MAX_ADJ_AMOUNT)  pid->x_s2 = MAX_ADJ_AMOUNT;
  if(pid->x_s2 < -MAX_ADJ_AMOUNT)  pid->x_s2 = -MAX_ADJ_AMOUNT;
  // *****************************CODE_END*****************************



  /*
   * Pitch inner PID:
   * Definition:
   * Input:
   * Gyro value: gyro_rad->gy
   * Setpoint (from the outer PID): pid->y_s1
   *
   * Intermediary variables:
   * Loop period (loop executed every pid->ts seconds): pid->ts
   * Integral variable (store here the error's integral): pid_y_integ2
   * Integral should be limited to the interval [-pid->y_i2_limit, pid->y_i2_limit]
   * Derivative variable (store here the derivative): deriv
   * Store here the error's previous value: pid_y_pre_error2
   * Store here the derivative's previous value - after filtering: pid_y_pre_deriv
   * Filter's law: deriv = pid_y_pre_deriv + (deriv - pid_y_pre_deriv)*D_FILTER_COFF;
   *
   * PID constants: pid->y_kp2, pid->y_ki2, pid->y_kd2
   *
   * Controller output: pid->y_s2
   */

  // ****************************CODE_BEGIN****************************
  error = pid->y_s1 - gyro_rad->gy;

  pid_z_integ2 += error*pid->ts;
  if(pid_z_integ2 > pid->y_i2_limit){
	  pid_z_integ2 = pid->y_i2_limit;
  } else if(pid_z_integ2 < pid->y_i2_limit){
	  pid_z_integ2= -pid->y_i2_limit;
  }

  deriv = (error - pid_y_pre_error2)*dt_recip;
  pid_y_pre_error2 = error;
  deriv = pid_y_pre_deriv + (deriv - pid_y_pre_deriv)*D_FILTER_COFF;
  pid_y_pre_deriv = deriv;

  pid->y_s2 = pid->y_kp2 * error + pid->y_ki2*pid_y_integ2 + pid->y_kd2*deriv;

  if(pid->y_s2 > MAX_ADJ_AMOUNT)  pid->y_s2 = MAX_ADJ_AMOUNT;
  if(pid->y_s2 < -MAX_ADJ_AMOUNT)  pid->y_s2 = -MAX_ADJ_AMOUNT;
  // *****************************CODE_END*****************************



  /*
   * Yaw inner PID:
   * Definition:
   * Input:
   * Gyro value: gyro_rad->gz
   * Setpoint (from the outer PID): pid->z_s1
   *
   * Intermediary variables:
   * Loop period (loop executed every pid->ts seconds): pid->ts
   * Integral variable (store here the error's integral): pid_z_integ2
   * Integral should be limited to the interval [-pid->z_i2_limit, pid->z_i2_limit]
   * Derivative variable (store here the derivative): deriv
   * Store here the error's previous value: pid_z_pre_error2
   * Store here the derivative's previous value - after filtering: pid_z_pre_deriv
   * Filter's law: deriv = pid_z_pre_deriv + (deriv - pid_z_pre_deriv)*D_FILTER_COFF;
   *
   * PID constants: pid->z_kp2, pid->z_ki2, pid->z_kd2
   *
   * Controller output: pid->z_s2
   */

  // ****************************CODE_BEGIN****************************
  error = pid->z_s1 - gyro_rad->gz;
  pid_z_integ2 += error*pid->ts;
  if(pid_z_integ2 > pid->z_i2_limit)
    pid_z_integ2 = pid->z_i2_limit;
  else if(pid_z_integ2 < -pid->z_i2_limit)
    pid_z_integ2 = -pid->z_i2_limit;

  deriv = (error - pid_z_pre_error2)*dt_recip;
  pid_z_pre_error2 = error;

  pid->z_s2 = pid->z_kp2*error + pid->z_ki2*pid_z_integ2 + pid->z_kd2*deriv;

  if(pid->z_s2 > MAX_ADJ_AMOUNT_YAW)  pid->z_s2 = MAX_ADJ_AMOUNT_YAW;
  if(pid->z_s2 < -MAX_ADJ_AMOUNT_YAW)  pid->z_s2 = -MAX_ADJ_AMOUNT_YAW;
  // *****************************CODE_END*****************************
  
#ifdef MOTOR_DC

  motor_thr = ((int16_t) (0.05f*(float)gTHR + 633.333f));           //Remocon Devo7E >> 630 to 1700
  //motor_thr =((int16_t) (0.333f*(float)gTHR + 633.33f));           //Remocon Devo7E >> 630 to 1700
  
#endif
  
#ifdef MOTOR_ESC
  
  motor_thr = 0.28f*gTHR + 750.0f;                 //TGY-i6 remocon and external ESC STEVAL-ESC001V1
//    motor_thr = ((int16_t) (0.28f*(float)gTHR + 850.0f));                 //TGY-i6 remocon and external ESC Afro12A

#endif

  motor_pwm->motor1_pwm = motor_thr - pid->x_s2 - pid->y_s2 + pid->z_s2 + MOTOR_OFF1;
  motor_pwm->motor2_pwm = motor_thr + pid->x_s2 - pid->y_s2 - pid->z_s2 + MOTOR_OFF2;
  motor_pwm->motor3_pwm = motor_thr + pid->x_s2 + pid->y_s2 + pid->z_s2 + MOTOR_OFF3;
  motor_pwm->motor4_pwm = motor_thr - pid->x_s2 + pid->y_s2 - pid->z_s2 + MOTOR_OFF4;

}

void PIDOuterLoopFrameTrans(P_PI_PIDControlTypeDef *pid, EulerAngleTypeDef *euler_ahrs)
{
  float cosx;
  
  cosx = cos(euler_ahrs->thx);
  pid->y_s1 = cosx*pid->y_s1;

}

/*
 * servo_ctrl.h
 *
 *  Created on: Sep 29, 2019
 *      Author: Jaehun Heo
 */

#ifndef INC_SERVO_CTRL_H_
#define INC_SERVO_CTRL_H_

#define PULSE_L (550)
#define PULSE_H (2350)
#define MAX_SPEED (50) //5 degrees per 20ms
#define MIN_SPEED (10) //1 degrees per 20ms
#define CW (-1)
#define CCW (1)
#define BASE_SPEED (5) //0.5 degrees per 20ms
#define ACC_RATIO (0.125)

void servo_initialize(int _servo_num);					// 서보모터 초기화
void servo_run(int _servo_num);										// 회전 시작
void servo_stop(int _servo_num);										// 회전 정지
void servo_update();									// 동작 업데이트
void servo_speed_update(int _servo_num);
void servo_set_angle(int _servo_num, int _angle);						// 목표 각도 설정
void servo_set_speed(int _servo_num, int _speed);						// 속도 설정
void servo_set_direction(int _servo_num, int _direction);				// 방향 설정
void servo_goto(int _servo_num, int _val, int _speed);					// 목표 각도로 회전
void servo_move(int _servo_num, int _val, int _direction, int _speed);	// 목표 회전량만큼 회전
int servo_is_running(int _servo_num);									// 서보 회전 완료여부 확인


#endif /* INC_SERVO_CTRL_H_ */

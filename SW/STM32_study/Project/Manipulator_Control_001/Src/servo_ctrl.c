/*
 * robot_arm.c
 *
 *  Created on: 2019 Oct. 17
 *  Original Author: Jaehun Heo
 *  Edited by Sean Kim
 */

#include "tim.h"
#include "servo_ctrl.h"

TIM_TypeDef *tim_reg;
//int cur_angle;			// 현재 각도, 0 ~ 1800 (0deg ~ 180deg)
//int dst_angle;			// 목표 각도
//int cur_speed;			// 현재 속도
//int dst_speed;			// 목표 속도
//int total_steps;		    // 전체 운행 스텝 수
//int acc_steps;			// 가속 구간 스텝 수
//int fixed_speed_steps;	// 정속 구간 스텝 수
//int cur_steps;			// 현재 스텝
//int acc;				    // 가속도
//int delta;				// 목표 각도까지 남은 값
//int direction;			// -1: CW, 1: CCW
//int run;				    // 0: 정지, 1: 동작

typedef struct {
	int cur_angle;			// 현재 각도, 0 ~ 1800 (0deg ~ 180deg)
	int dst_angle;			// 목표 각도
	int cur_speed;			// 현재 속도 degree per 20ms
	int dst_speed;			// 목표 속도 degree per 20ms
	int total_steps;		// 전체 운행 스텝 수
	int acc_steps;			// 가속 구간 스텝 수
	int fixed_speed_steps;	// 정속 구간 스텝 수
	int cur_steps;			// 현재 스텝
	int acc;				// 가속도
	int delta;				// 목표 각도까지 남은 값
	int direction;			// -1: CW, 1: CCW
	int run;				// 0: 정지, 1: 동작
	int enable;
} Servo;                    //구조체 변수 생성

Servo servo[3];


void servo_initialize(int _servo_num)
{
	if (!servo[0].enable && !servo[1].enable && !servo[2].enable)
	{
		HAL_TIM_Base_Start_IT(&htim3);
		tim_reg = htim3.Instance;
	}

	switch (_servo_num)
	{
	case 0:
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1); //PA6
		break;
	case 1:
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2); //PB0
		break;
	case 2:
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3); //PC7
		break;
	}

	servo[_servo_num].cur_angle = 0;
}

void servo_run(int _servo_num)
{
	if (servo[_servo_num].delta != 0)
	{
		servo[_servo_num].run = 1;
	}
	else // 목표 각도에 도달했을때
	{
		servo[_servo_num].run = 0;
	}
}

void servo_stop(int _servo_num)
{
	servo[_servo_num].delta = 0;
	servo[_servo_num].run = 0;
}

void servo_update()
{
	int pulse = 0;
	int i = 0;

	for (i = 0; i < 3; i++) //servo[0]~servo[2]
	{
		if (servo[i].run == 1 && servo[i].delta != 0) //서보를 작동시킨다는 명령이 주어지고 목표 각도까지의 남은 각도가 0이 아닐때
		{
			servo_speed_update(i);

			if (servo[i].delta > servo[i].cur_speed)
			{
				pulse = servo[i].cur_speed * servo[i].direction;
			}
			else
			{
				pulse = servo[i].delta * servo[i].direction;
			}

			servo[i].cur_angle += pulse;

			switch(i)
			{
			case 0:
				tim_reg->CCR1 = PULSE_L + servo[i].cur_angle;
				break;
			case 1:
				tim_reg->CCR2 = PULSE_L + servo[i].cur_angle;
				break;
			case 2:
				tim_reg->CCR3 = PULSE_L + servo[i].cur_angle;
				break;
			}

			servo[i].delta -= pulse * servo[i].direction;
			servo[i].cur_steps += 1;
		}
		else
		{
			servo[i].run = 0;
			servo[i].cur_steps = 0;
		}
	}
}

void servo_speed_update(int _servo_num) //
{
	int i = _servo_num;

	if ((servo[i].cur_steps > servo[i].acc_steps - 1) && (servo[i].cur_steps <= servo[i].acc_steps + servo[i].fixed_speed_steps - 1))
	{
		servo[i].cur_speed = servo[i].dst_speed;
	}
	else if ((servo[i].cur_steps > 0) && (servo[i].cur_steps <= servo[i].acc_steps - 1))
	{
		servo[i].cur_speed += servo[i].acc;
	}
	else if ((servo[i].cur_steps > servo[i].acc_steps + servo[i].fixed_speed_steps - 1) && (servo[i].cur_steps <= servo[i].total_steps - 1))
	{
		servo[i].cur_speed -= servo[i].acc;
	}
	else if (servo[i].cur_steps == 0)
	{
		servo[i].cur_speed = BASE_SPEED;
	}
}

void servo_set_angle(int _servo_num, int _angle)
{
	int i = _servo_num;

	if (_angle > 1800)
	{
		_angle = 1800;
	}
	else if (_angle < 0)
	{
		_angle = 0;
	}

	servo[i].dst_angle = _angle;
	servo[i].delta = servo[i].dst_angle - servo[i].cur_angle;

	if (servo[i].delta < 0)
	{
		servo_set_direction(i, CW);
		servo[i].delta *= -1;
	}
	else if(servo[i].delta > 0)
	{
		servo_set_direction(i, CCW);
	}

	servo[i].total_steps = servo[i].delta / (servo[i].dst_speed - ACC_RATIO * (servo[i].dst_speed - BASE_SPEED));
	servo[i].acc_steps = ACC_RATIO * servo[i].total_steps;
	servo[i].fixed_speed_steps = servo[i].total_steps - 2 * servo[i].acc_steps;
	servo[i].acc = (servo[i].dst_speed - BASE_SPEED) / servo[i].acc_steps;
}

void servo_set_speed(int _servo_num, int _speed) //타겟 스피드를 결정
{
	int i = _servo_num;

	if (_speed > MAX_SPEED)
	{
		servo[i].dst_speed = MAX_SPEED;
	}
	else if (_speed < MIN_SPEED)
	{
		servo[i].dst_speed = MIN_SPEED;
	}
	else
	{
		servo[i].dst_speed = _speed;
	}
}

void servo_set_direction(int _servo_num, int _direction)
{
	servo[_servo_num].direction = _direction;
}

void servo_goto(int _servo_num, int _val, int _speed)
{
	if (_speed != 0)
	{
		servo_set_speed(_servo_num, _speed);
	}

	servo_set_angle(_servo_num, _val);
	servo_run(_servo_num);
}

void servo_move(int _servo_num, int _val, int _direction, int _speed)
{
	if (_speed != 0)
	{
		servo_set_speed(_servo_num, _speed);
	}

	servo_set_direction(_servo_num, _direction);
	servo_set_angle(_servo_num, servo[_servo_num].cur_angle + _val * _direction);
	servo_run(_servo_num);
}

int servo_is_running(int _servo_num)
{
	return servo[_servo_num].run;
}

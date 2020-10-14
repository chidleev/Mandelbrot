#version 330 core
out vec4 fColor;

in vec4 position;

uniform mat4 transform;
uniform int max_iter;
uniform float x;
uniform float y;
uniform float time;


vec2 cplx_mult(in vec2 a, in vec2 b)
{
	vec2 temp;
	temp.x = a.x * b.x - a.y * b.y;
	temp.y = a.y * b.x + a.x * b.y;
	return temp;
}

vec2 cplx_div(in vec2 a, in vec2 b)
{
	vec2 temp;
	temp.x = (a.x * b.x + a.y * b.y) / (b.x * b.x + b.y * b.y);
	temp.y = (a.y * b.x + a.x * b.y) / (b.x * b.x + b.y * b.y);
	return temp;
}

vec2 cplx_exp(in vec2 a)
{
	vec2 temp;
	temp.x = exp(a.x) * cos(a.y);
	temp.y = exp(a.x) * sin(a.y);
	return temp;
}

vec2 cplx_ln(in vec2 a)
{
	vec2 temp;
	temp.x = log(length(a));
	temp.y = atan(a.x, a.y);
	return temp;
}

vec2 cplx_pow(in vec2 a, int p)
{
	vec2 temp = vec2(1, 0);
	for (int i = 0; i < abs(p); i++)
	{
		temp = cplx_mult(temp, a);
	}
	if (p < 0) temp = cplx_div(vec2(1, 0), temp);
	return temp;
}


void main()
{
	vec4 pixel = transform * position;

	vec2 cur_num = vec2(0, 0);
	vec2 last_num;
	vec2 const_num = vec2(x, y);

	float iterations = 0;
	float sum_distance = 0;

	float last_angle = 0;

	cur_num = pixel.xy;
	float t = 200;

	for (int i = 0; (i < t) && (length(cur_num) < 10.f); i++)
	{
		last_num = cur_num;

		cur_num = cplx_pow(cur_num, -2) + const_num;

		iterations += 1/t;
		sum_distance += distance(cur_num, last_num);
	}

	sum_distance /= (50);

	float cv1 = mix(iterations, sum_distance, 0.5);
	float cv2 = iterations * sum_distance;
	
	
	vec4 iter_white_mask = mix(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), iterations);
	vec4 iter_black_mask = mix(vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), iterations);
	
	vec4 dist_white_mask = mix(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), sum_distance);
	vec4 dist_black_mask = mix(vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), sum_distance);
	

	vec4 anim1 = vec4(0.5 * cos(10 * iterations + time/2) + 0.5, 0.5 * cos(10 * iterations + time/2 - 1.57) + 0.5, 0.5 * cos(10 * iterations + time/2 - 1.57) + 0.5, 1);
	vec4 anim2 = vec4(0.5 * cos(10 * cv2 + time) + 0.5, 0.5 * cos(10 * cv2 + time) + 0.5, 0.5 * cos(10 * cv2 + time) + 0.5, 1);
	vec4 anim3 = vec4(0.5 * cos(5 * cv1 - time) + 0.5, 0.5 * cos(5 * cv1 - time) + 0.5, 0.5 * cos(5 * cv1 - time) + 0.5, 1);


	fColor = vec4(1, 1, 1, 1) * iter_white_mask + anim1 * iter_black_mask;
}
#version 330 core
out vec4 fColor;

in vec4 position;

uniform mat4 transform;
uniform int max_iter;
uniform float x;
uniform float y;
uniform float time;


vec2 mult(in vec2 a, in vec2 b)
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
	vec2 temp = vec2(1, 1);
	for (int i = 0; i < abs(p); i++)
	{
		temp = mult(temp, a);
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

	//cur_num = pixel.xy;

	for (int i = 0; (i < max_iter) && (length(cur_num) < 10.f); i++)
	{
		last_num = cur_num;

		cur_num = cplx_pow(cur_num, 2);
		cur_num += pixel.xy;

		iterations += 1;

		sum_distance += distance(cur_num, last_num);
	}

	iterations /= max_iter;
	sum_distance /= (50);

	float cv1 = mix(iterations, sum_distance, 0.5);
	float cv2 = iterations * sum_distance;

	fColor = vec4(cos(4 * cv2 + time + 0.5), cos(4 * cv2 + time), cos(4 * cv2 + time - 0.5), 1);
}
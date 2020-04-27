#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw	= -90.0f;
	float pitch	= 0.0f;
	float speed	= 2.5f;
	float sens	= 0.1f;
	float fov	= 45.0f;

	// Vector Constructor
	Camera(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f)) : front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		pos = _pos;
		up = _up;
		worldUp = _up;
		
		UpdateCameraVectors();
	}
	// Destructor
	~Camera() {};

	// Return View Matrix
	glm::mat4 GetViewMatrix()
	{
		return (glm::lookAt(pos, pos + front, up));
	}

	// Process Keyboard Input
	void ProcessKeyBoard(Camera_Movement _direction, float _deltaTime)
	{
		float velocity = speed * _deltaTime;
		if (_direction == FORWARD)
		{
			pos += front * velocity;
		}
		if (_direction == BACKWARD)
		{
			pos -= front * velocity;
		}
		if (_direction == LEFT)
		{
			pos -= right * velocity;
		}
		if (_direction == RIGHT)
		{
			pos += right * velocity;
		}
	}
	// Process Mouse Input
	void ProcessMouseMovement(float _xoffset, float _yoffset, GLboolean constrainPitch = true)
	{
		_xoffset *= sens;
		_yoffset *= sens;

		yaw += _xoffset;
		pitch += _yoffset;

		// Gimbal Lock
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		UpdateCameraVectors();
	}
	// Process Mouse Scroll
	void ProcessMouseScroll(float _yoffset)
	{
		if (fov - _yoffset > 1.0f && fov - _yoffset < 45.0f)
		{
			fov -= _yoffset;
		}
		if (fov <= 1.0f)
		{
			fov = 1.0f;
		}
		if (fov >= 45.0f)
		{
			fov = 45.0f;
		}
	}

	// Update Camera
	void UpdateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 dir;
		dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		dir.y = sin(glm::radians(pitch));
		dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(dir);
		// Also re-calculate the Right and Up vector
		right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = glm::normalize(glm::cross(right, front));
	}
};

#endif
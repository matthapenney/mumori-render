#pragma once
#include "renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {

public:
	Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 globalUp, float FOV);
	~Camera();

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	
	void ProcessKeyboard(GLFWwindow* window, float& deltaTime);
	void MouseCallback(GLFWwindow* window, double& xpos, double& ypos);
	void ScrollCallback(GLFWwindow* window, double& xoffset, double& yoffset);
	
	const glm::vec3& GetCameraFront() const;
	const glm::vec3& GetCameraPosition() const;
	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetViewProjectionMatrix() const;
	void RecalculateViewMatrix();
private:
	glm::vec3 m_cameraPosition = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	float m_FOV;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;
};

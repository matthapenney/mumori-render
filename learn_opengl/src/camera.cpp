#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 globalUp, float FOV)
	:
	m_cameraPosition(cameraPosition),
	m_cameraFront(cameraFront),
	m_cameraUp(globalUp),
	m_FOV(FOV),
	m_ViewMatrix(1.0f)
{
	// Gram-Schmidt - generate view/camera space vectors
	//m_cameraDirection = glm::normalize(m_cameraFront - m_cameraPosition);
	//m_cameraRight = glm::normalize(glm::cross(m_cameraUp, m_cameraDirection));
	//m_cameraUp = glm::cross(m_cameraFront, m_cameraRight);
	m_ViewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
	m_ProjectionMatrix = glm::perspective(glm::radians(FOV), 8.0f / 4.5f, 0.1f, 100.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

Camera::~Camera()
{
}

void Camera::SetPosition(const glm::vec3& position) 
{
	m_cameraPosition = position;
	RecalculateViewMatrix();
}

void Camera::SetRotation(const glm::vec3& rotation) 
{
	m_cameraFront = rotation; 
	RecalculateViewMatrix();
}

void Camera::ProcessKeyboard(GLFWwindow* window, float& deltaTime)
{
	const float cameraSpeed = 3.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_cameraPosition += cameraSpeed * m_cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_cameraPosition -= cameraSpeed * m_cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_cameraPosition -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_cameraPosition += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	RecalculateViewMatrix();
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void Camera::ScrollCallback(GLFWwindow* window, double& xoffset, double& yoffset)
{
	m_FOV -= (float)yoffset;
	if (m_FOV < 1.0f)
		m_FOV = 1.0f;
	if (m_FOV > 75.0f)
		m_FOV = 75.0f;
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), 8.0f / 4.5f, 0.1f, 100.0f);
}

void Camera::RecalculateViewMatrix()
{
	m_ViewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

const glm::vec3& Camera::GetCameraPosition() const
{
	return m_cameraPosition;
}

const glm::vec3& Camera::GetCameraFront() const
{
	return m_cameraFront;
}

const glm::mat4& Camera::GetViewProjectionMatrix() const
{
	return m_ViewProjectionMatrix;
}

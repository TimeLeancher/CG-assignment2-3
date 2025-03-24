#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/intersect.hpp>
using namespace glm;

// -------------------------------------------------
// Global Variables
// -------------------------------------------------
int Width = 512;
int Height = 512;
std::vector<float> OutputImage;
// -------------------------------------------------

// 카메라 관련 변수 [cite: 6, 7]
vec3 eye = vec3(0, 0, 0); // 시점
vec3 u = vec3(1, 0, 0); // u 벡터
vec3 v = vec3(0, 1, 0); // v 벡터
vec3 w = vec3(0, 0, 1); // w 벡터 (카메라 방향은 -w)
float l = -0.1f; // left
float r = 0.1f;  // right
float b = -0.1f; // bottom
float t = 0.1f;  // top
float d = 0.1f;  // distance to the image plane

// 장면 구성 요소 [cite: 5, 6]
struct Plane {
	float y; // 평면의 y 좌표
};

struct Sphere {
	vec3 center; // 구의 중심
	float radius; // 구의 반지름
};

Plane plane = { -2.0f };
Sphere sphere1 = { vec3(-4, 0, -7), 1.0f };
Sphere sphere2 = { vec3(0, 0, -7), 2.0f };
Sphere sphere3 = { vec3(4, 0, -7), 1.0f };

// 광선 구조체
struct Ray {
	vec3 origin;    // 광선의 시작점
	vec3 direction; // 광선의 방향
};

// 광선 생성 함수
Ray generateRay(int x, int y) {
	float ndcx = (float)x / (float)Width;
	float ndcy = (float)y / (float)Height;
	float p_x = l + (r - l) * ndcx;
	float p_y = b + (t - b) * ndcy;

	return {
		eye,
		normalize(-d * w + p_x * u + p_y * v)
	};
}

// 광선-평면 교차 함수
float intersectRayPlane(const Ray& ray, const Plane& plane) {
	if (abs(ray.direction.y) < 1e-6) {
		return -1.0f; // 광선이 평면과 평행
	}
	float t = (plane.y - ray.origin.y) / ray.direction.y;
	if (t > 0) {
		return t;
	}
	return -1.0f; // 교차점이 없음
}

// 광선-구 교차 함수
float intersectRaySphere(const Ray& ray, const Sphere& sphere) {
	vec3 oc = ray.origin - sphere.center;
	float a = dot(ray.direction, ray.direction);
	float b = 2.0f * dot(oc, ray.direction);
	float c = dot(oc, oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) {
		return -1.0f; // 광선이 구를 지나가지 않음
	}
	else {
		float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
		if (t > 0) {
			return t;
		}
		return -1.0f;
	}
}

// 가장 가까운 교차점 찾기 함수
std::pair<int, float> findClosestIntersection(const Ray& ray) {
	float closestT = INFINITY;
	int closestObjectIndex = -1; // 0: plane, 1: sphere1, 2: sphere2, 3: sphere3

	float tPlane = intersectRayPlane(ray, plane);
	if (tPlane > 0 && tPlane < closestT) {
		closestT = tPlane; // 교차점이 평면일 경우
		closestObjectIndex = 0;
	}

	float tSphere1 = intersectRaySphere(ray, sphere1);
	if (tSphere1 > 0 && tSphere1 < closestT) {
		closestT = tSphere1; // 교차점이 구1일 경우
		closestObjectIndex = 1;
	}

	float tSphere2 = intersectRaySphere(ray, sphere2);
	if (tSphere2 > 0 && tSphere2 < closestT) {
		closestT = tSphere2; // 교차점이 구2일 경우
		closestObjectIndex = 2;
	}

	float tSphere3 = intersectRaySphere(ray, sphere3);
	if (tSphere3 > 0 && tSphere3 < closestT) {
		closestT = tSphere3; //	교차점이 구3일 경우
		closestObjectIndex = 3;
	}

	return std::make_pair(closestObjectIndex, closestT);
}


void render() {
	OutputImage.clear();
	for (int y = 0; y < Height; ++y) {
		for (int x = 0; x < Width; ++x) {
			Ray ray = generateRay(x, y);
			std::pair<int, float> closestIntersection = findClosestIntersection(ray);

			if (closestIntersection.first != -1) {
				// 교차하는 물체가 있을 경우 흰색
				OutputImage.push_back(1.0f); // R
				OutputImage.push_back(1.0f); // G
				OutputImage.push_back(1.0f); // B
			}
			else {
				// 교차하는 물체가 없을 경우 검은색
				OutputImage.push_back(0.0f); // R
				OutputImage.push_back(0.0f); // G
				OutputImage.push_back(0.0f); // B
			}
		}
	}
}
/*
 void render()
{
	//Create our image. We don't want to do this in 
	//the main loop since this may be too slow and we 
	//want a responsive display of our beautiful image.
	//Instead we draw to another buffer and copy this to the 
	//framebuffer using glDrawPixels(...) every refresh
	OutputImage.clear();
	for (int j = 0; j < Height; ++j) 
	{
		for (int i = 0; i < Width; ++i) 
		{
			// ---------------------------------------------------
			// --- Implement your code here to generate the image
			// ---------------------------------------------------

			// draw a red rectangle in the center of the image   이 부분 지우고 하기 
			vec3 color = glm::vec3(0.5f, 0.5f, 0.5f); // grey color [0,1] in RGB channel
			
			if (i > Width / 4 && i < 3 * Width / 4 
				&& j > Height / 4 && j < 3 * Height / 4)
			{
				color = glm::vec3(1.0f, 0.0f, 0.0f); // red color [0,1] in RGB channel
			}
			
			// set the color
			OutputImage.push_back(color.x); // R
			OutputImage.push_back(color.y); // G
			OutputImage.push_back(color.z); // B
		}
	}
}

*/
void resize_callback(GLFWwindow*, int nw, int nh) 
{
	//This is called in response to the window resizing.
	//The new width and height are passed in so we make 
	//any necessary changes:
	Width = nw;
	Height = nh;
	//Tell the viewport to use all of our screen estate
	glViewport(0, 0, nw, nh);

	//This is not necessary, we're just working in 2d so
	//why not let our spaces reflect it?
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, static_cast<double>(Width)
		, 0.0, static_cast<double>(Height)
		, 1.0, -1.0);

	//Reserve memory for our render so that we don't do 
	//excessive allocations and render the image
	OutputImage.reserve(Width * Height * 3);
	render();
}


int main(int argc, char* argv[])
{
	// -------------------------------------------------
	// Initialize Window
	// -------------------------------------------------

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//We have an opengl context now. Everything from here on out 
	//is just managing our window or opengl directly.

	//Tell the opengl state machine we don't want it to make 
	//any assumptions about how pixels are aligned in memory 
	//during transfers between host and device (like glDrawPixels(...) )
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//We call our resize function once to set everything up initially
	//after registering it as a callback with glfw
	glfwSetFramebufferSizeCallback(window, resize_callback);
	resize_callback(NULL, Width, Height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// -------------------------------------------------------------
		//Rendering begins!
		glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
		//and ends.
		// -------------------------------------------------------------

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//Close when the user hits 'q' or escape
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

//
// declarations
//
camera_t* camera;
float camera_vel = 1.0f;
Quad_t* quad;
OBJModel_t* obj;
// model-to-world matrices
mat4f Mtire;
float tire_angle = 0;			// rad
float tire_angle_vel = fPI / 4; // rad/s
mat4f Mquad;
// world-to-view matrix
mat4f Mview;
// projection matrix
mat4f Mproj;

//
// called at initialization
//
void initScene()
{
	// create camera
	camera = new camera_t(fPI / 4,				/*field-of-view*/
		(float)width / height,	/*aspect ratio*/
		1.0f,					/*z-near plane (everything closer will be clipped/removed)*/
		500.0f);				/*z-far plane (everything further will be clipped/removed)*/
	camera->moveTo({ 0, 0, 5 });

	// create objects
	quad = new Quad_t(g_Device);
	obj = new OBJModel_t("../../assets/tyre/Tyre.obj", g_Device);

	// init matrices
	Mquad = linalg::mat4f_identity;
	Mtire = linalg::mat4f_identity;

	Mview = mat4f::translation(0, 0, -5);
	Mproj = mat4f::GL_projection(45.0 * 3.14 / 180.0, (float)width / height, 1, 500);
}

//
// called every frame
//
void updateScene(float dt)
{
	// basic camera control
	if (inputHandler->IsKeyPressed(Keys::Up) || inputHandler->IsKeyPressed(Keys::W))
		camera->move({ 0.0f, 0.0f, -camera_vel *dt });
	if (inputHandler->IsKeyPressed(Keys::Down) || inputHandler->IsKeyPressed(Keys::S))
		camera->move({ 0.0f, 0.0f, camera_vel *dt });
	if (inputHandler->IsKeyPressed(Keys::Right) || inputHandler->IsKeyPressed(Keys::D))
		camera->move({ camera_vel *dt, 0.0f, 0.0f });
	if (inputHandler->IsKeyPressed(Keys::Left) || inputHandler->IsKeyPressed(Keys::A))
		camera->move({ -camera_vel *dt, 0.0f, 0.0f });

	tire_angle += tire_angle_vel * dt;
	Mtire = mat4f::rotation(tire_angle, 0, 1, 0);
	Mquad = mat4f::rotation(-tire_angle, 0, 1, 0);
}

//
// called every frame, after update
//
void renderScene()
{
	Mview = camera->get_WorldToViewMatrix();
	Mproj = camera->get_ProjectionMatrix();

	quad->MapShaderBuffers(g_DeviceContext, g_MatrixBuffer, Mquad, Mview, Mproj);
	quad->render(g_DeviceContext);

	obj->MapShaderBuffers(g_DeviceContext, g_MatrixBuffer, Mtire, Mview, Mproj);
	obj->render(g_DeviceContext);
}

//
// called at termination
//
void releaseScene()
{
	SAFE_DELETE(quad);
	SAFE_DELETE(obj);
}

#endif
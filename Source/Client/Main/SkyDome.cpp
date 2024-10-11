#include "stdafx.h"
#include "SkyDome.h"
#include "MapManager.h"

CSkyDome gSkyDome;

CSkyDome::CSkyDome()
{
	memset(this->SkyDomeTable, 0, sizeof(this->SkyDomeTable));

	this->SkyColor.R = 1.0f;
	this->SkyColor.G = 1.0f;
	this->SkyColor.B = 1.0f;
}

CSkyDome::~CSkyDome()
{
	if (this->Vertices)
	{
		delete[] this->Vertices;

		this->Vertices = NULL;
	}
}

void CSkyDome::Init()
{
	this->GenerateSkyDomeStructure(600.0f, 5.0f, 5.0f, 1.0f, 1.0f);

	SetCompleteHook(0xE8, 0x00525C5F, &this->DrawSkyDome);
}

void CSkyDome::GenerateSkyDomeStructure(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	int theta, phi;

	// Make sure our vertex array is clear
	if (this->Vertices)
	{
		delete[] this->Vertices;

		this->Vertices = NULL;

		this->NumVertices = 0;
	}

	// Initialize our Vertex array
	this->NumVertices = (int)((360 / dtheta) * (90 / dphi) * 4);

	this->Vertices = new VERTEX[this->NumVertices];

	ZeroMemory(this->Vertices, sizeof(VERTEX) * this->NumVertices);

	// Used to calculate the UV coordinates
	float vx, vy, vz, mag;

	// Generate the dome
	int n = 0;

	for (phi = 0; phi <= 90 - dphi; phi += (int)dphi)
	{
		for (theta = 0; theta <= 360 - dtheta; theta += (int)dtheta)
		{
			// Calculate the vertex at phi, theta
			this->Vertices[n].x = radius * sinf(phi * DTOR) * cosf(DTOR * theta);

			this->Vertices[n].y = radius * sinf(phi * DTOR) * sinf(DTOR * theta);

			this->Vertices[n].z = radius * cosf(phi * DTOR);

			// Create a vector from the origin to this vertex
			vx = this->Vertices[n].x;

			vy = this->Vertices[n].y;

			vz = this->Vertices[n].z;

			// Normalize the vector
			mag = (float)sqrt(SQR(vx) + SQR(vy) + SQR(vz));

			vx /= mag;

			vy /= mag;

			vz /= mag;

			// Calculate the spherical texture coordinates
			this->Vertices[n].u = hTile * (float)(atan2(vx, vz) / (PI * 2)) + 0.5f;

			this->Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;

			n++;

			// Calculate the vertex at phi+dphi, theta
			this->Vertices[n].x = radius * sinf((phi + dphi) * DTOR) * cosf(theta * DTOR);

			this->Vertices[n].y = radius * sinf((phi + dphi) * DTOR) * sinf(theta * DTOR);

			this->Vertices[n].z = radius * cosf((phi + dphi) * DTOR);

			// Calculate the texture coordinates
			vx = this->Vertices[n].x;

			vy = this->Vertices[n].y;

			vz = this->Vertices[n].z;

			mag = (float)sqrt(SQR(vx) + SQR(vy) + SQR(vz));

			vx /= mag;

			vy /= mag;

			vz /= mag;

			this->Vertices[n].u = hTile * (float)(atan2(vx, vz) / (PI * 2)) + 0.5f;

			this->Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;

			n++;

			// Calculate the vertex at phi, theta+dtheta
			this->Vertices[n].x = radius * sinf(DTOR * phi) * cosf(DTOR * (theta + dtheta));

			this->Vertices[n].y = radius * sinf(DTOR * phi) * sinf(DTOR * (theta + dtheta));

			this->Vertices[n].z = radius * cosf(DTOR * phi);

			// Calculate the texture coordinates
			vx = this->Vertices[n].x;

			vy = this->Vertices[n].y;

			vz = this->Vertices[n].z;

			mag = (float)sqrt(SQR(vx) + SQR(vy) + SQR(vz));

			vx /= mag;

			vy /= mag;

			vz /= mag;

			this->Vertices[n].u = hTile * (float)(atan2(vx, vz) / (PI * 2)) + 0.5f;

			this->Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;

			n++;

			if (phi > -90 && phi < 90)
			{
				// Calculate the vertex at phi+dphi, theta+dtheta
				this->Vertices[n].x = radius * sinf((phi + dphi) * DTOR) * cosf(DTOR * (theta + dtheta));

				this->Vertices[n].y = radius * sinf((phi + dphi) * DTOR) * sinf(DTOR * (theta + dtheta));

				this->Vertices[n].z = radius * cosf((phi + dphi) * DTOR);

				// Calculate the texture coordinates
				vx = this->Vertices[n].x;

				vy = this->Vertices[n].y;

				vz = this->Vertices[n].z;

				mag = (float)sqrt(SQR(vx) + SQR(vy) + SQR(vz));

				vx /= mag;

				vy /= mag;

				vz /= mag;

				this->Vertices[n].u = hTile * (float)(atan2(vx, vz) / (PI * 2)) + 0.5f;

				this->Vertices[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;

				n++;
			}
		}
	}

	// Fix the problem at the seam
	for (int i = 0; i < this->NumVertices - 3; i++)
	{
		if (this->Vertices[i].u - this->Vertices[i + 1].u > 0.9f)
		{
			this->Vertices[i + 1].u += 1.0f;
		}

		if (this->Vertices[i + 1].u - this->Vertices[i].u > 0.9f)
		{
			this->Vertices[i].u += 1.0f;
		}

		if (this->Vertices[i].u - this->Vertices[i + 2].u > 0.9f)
		{
			this->Vertices[i + 2].u += 1.0f;
		}

		if (this->Vertices[i + 2].u - this->Vertices[i].u > 0.9f)
		{
			this->Vertices[i].u += 1.0f;
		}

		if (this->Vertices[i + 1].u - this->Vertices[i + 2].u > 0.9f)
		{
			this->Vertices[i + 2].u += 1.0f;
		}

		if (this->Vertices[i + 2].u - this->Vertices[i + 1].u > 0.9f)
		{
			this->Vertices[i + 1].u += 1.0f;
		}

		if (this->Vertices[i].v - this->Vertices[i + 1].v > 0.8f)
		{
			this->Vertices[i + 1].v += 1.0f;
		}

		if (this->Vertices[i + 1].v - this->Vertices[i].v > 0.8f)
		{
			this->Vertices[i].v += 1.0f;
		}

		if (this->Vertices[i].v - this->Vertices[i + 2].v > 0.8f)
		{
			this->Vertices[i + 2].v += 1.0f;
		}

		if (this->Vertices[i + 2].v - this->Vertices[i].v > 0.8f)
		{
			this->Vertices[i].v += 1.0f;
		}

		if (this->Vertices[i + 1].v - this->Vertices[i + 2].v > 0.8f)
		{
			this->Vertices[i + 2].v += 1.0f;
		}

		if (this->Vertices[i + 2].v - this->Vertices[i + 1].v > 0.8f)
		{
			this->Vertices[i + 1].v += 1.0f;
		}
	}
}

void CSkyDome::LoadImages()
{
	for (int i = 0; i < MAX_MAPS; i++)
	{
		if (!gMapManager.GetSkyDome(i))
		{
			this->SkyDomeTable[i] = false;

			continue;
		}

		sprintf_s(this->TexturePath, SKY_TOP_TEXTURE, i + 1);

		if (!FileExists(this->TexturePath))
		{
			this->SkyDomeTable[i] = false;

			continue;
		}

		stbi_set_flip_vertically_on_load(true);

		stb_ImageData* SkyImage = new stb_ImageData();

		SkyImage->data = stbi_load(this->TexturePath, &SkyImage->imageWidth, &SkyImage->imageHeight, &SkyImage->colorChannel, 0);

		this->texturesID[i] = this->LoadTexture(SkyImage->data, SkyImage->imageWidth, SkyImage->imageHeight, SkyImage->colorChannel);

		stbi_image_free(SkyImage->data);

		delete SkyImage;

		this->SkyDomeTable[i] = true;
	}
}

GLuint CSkyDome::LoadTexture(BYTE* imageBytes, int imageWidth, int imageHeight, int colorChannel)
{
	GLuint textureID;

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (colorChannel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes);
	}
	else if (colorChannel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBytes);
	}
	else if (colorChannel == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, imageBytes);
	}
	else
	{
		MessageBox(0, "Automatic Texture type recognition failed", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void CSkyDome::DrawSkyDome()
{
	((void(__cdecl*)()) 0x0046C3E0)();

	if (MAP_RANGE(World))
	{
		if (!gSkyDome.SkyDomeTable[World])
		{
			return;
		}

		gSkyDome.RenderSkyDome();
	}
}

void CSkyDome::RenderSkyDome()
{
	float x_char = *(float*)(Hero + 0x10);
	float y_char = *(float*)(Hero + 0x14);
	float z_char = *(float*)(Hero + 0x12C) + *(float*)(Hero + 0x18) - 200.0f;

	glPushMatrix();

	glTranslatef(x_char, y_char, z_char);

	glRotatef(WorldTime * 0.001f, 0.0f, 0.0f, 1.0f);

	glScalef(4.0f, 4.0f, 3.0f);

	EnableAlphaTest(1);

	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // Setup The Ambient Light

	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // Setup The Diffuse Light

	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // Position The Light

	glEnable(GL_LIGHT1); // Enable Light One

	glColor4f(this->SkyColor.R, this->SkyColor.G, this->SkyColor.B, 0.5f); // Full Brightness. 50% Alpha

	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Set The Blending Function For Translucency

	glColor3d(this->SkyColor.R, this->SkyColor.G, this->SkyColor.B);

	glBindTexture(GL_TEXTURE_2D, this->texturesID[World]);

	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i < this->NumVertices; i++)
	{
		glTexCoord2f(this->Vertices[i].u, this->Vertices[i].v);

		glVertex3f(this->Vertices[i].x, this->Vertices[i].y, this->Vertices[i].z);
	}

	glEnd();

	glPopMatrix();

	DisableAlphaBlend();
}
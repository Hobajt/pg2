#include "pch.h"
#include "material.h"

const char Material::kDiffuseMapSlot = 0;
const char Material::kSpecularMapSlot = 1;
const char Material::kNormalMapSlot = 2;
const char Material::kOpacityMapSlot = 3;
const char Material::kRoughnessMapSlot = 4;
const char Material::kMetallicnessMapSlot = 5;
const char Material::kRMAMapSlot = 5;

Material::Material() {
	// defaultní materiál
	ambient_ = Color3f({ 0.1f, 0.1f, 0.1f });
	diffuse_ = Color3f({ 0.5f, 0.5f, 0.5f });
	specular_ = Color3f({ 0.6f, 0.6f, 0.6f });

	emission_ = Color3f({ 0.0f, 0.0f, 0.0f });

	reflectivity = static_cast<float>(0.99);
	shininess = 1.0f;
	roughness_ = 1.0f;
	metallicness = 0.0f;

	ior = -1.0f;

	memset(textures_, 0, sizeof(*textures_) * NO_TEXTURES);

	name_ = "default";
	shader_ = Shader::PHONG;
}

Material::Material(std::string& name, const Color3f& ambient, const Color3f& diffuse,
				   const Color3f& specular, const Color3f& emission, const float reflectivity,
				   const float shininess, const float ior, const Shader shader, Texture3u** textures, const int no_textures) {
	name_ = name;

	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;

	emission_ = emission;

	this->reflectivity = reflectivity;
	this->shininess = shininess;

	this->ior = ior;

	if (textures) {
		memcpy(textures_, textures, sizeof(textures) * no_textures);
	}
}

Material::~Material() {
	for (int i = 0; i < NO_TEXTURES; ++i) {
		if (textures_[i]) {
			delete[] textures_[i];
			textures_[i] = nullptr;
		};
	}
}

void Material::set_name(const char* name) {
	name_ = std::string(name);
}

std::string Material::name() const {
	return name_;
}

void Material::set_texture(const int slot, Texture3u* texture) {
	textures_[slot] = texture;
}

Texture3u* Material::texture(const int slot) const {
	return textures_[slot];
}

Shader Material::shader() const {
	return shader_;
}

void Material::set_shader(Shader shader) {
	shader_ = shader;
}

Color3f Material::ambient(const Coord2f* tex_coord) const {
	return ambient_;
}

Color3f Material::diffuse(const Coord2f* tex_coord) const {
	if (tex_coord) {
		Texture3u* texture = textures_[kDiffuseMapSlot];

		if (texture) {
			return Color3f(texture->texel(tex_coord->u, tex_coord->v));
		}
	}

	return diffuse_;
}

Color3f Material::specular(const Coord2f* tex_coord) const {
	if (tex_coord) {
		Texture3u* texture = textures_[kSpecularMapSlot];

		if (texture) {
			return Color3f(texture->texel(tex_coord->u, tex_coord->v));
		}
	}

	return specular_;
}

Color3f Material::bump(const Coord2f* tex_coord) const {
	if (tex_coord) {
		Texture3u* texture = textures_[kNormalMapSlot];

		if (texture) {
			return Color3f(texture->texel(tex_coord->u, tex_coord->v));
		}
	}

	return Color3f({ 0.5f, 0.5f, 1.0f }); // n = ( 0, 0, 1 )	
}

float Material::roughness(const Coord2f* tex_coord) const {

	if (tex_coord) {
		Texture3u* texture = textures_[kRoughnessMapSlot];

		if (texture) {
			return texture->texel(tex_coord->u, tex_coord->v).data[0] / 255.0f;
		}
	}

	return roughness_;
}

Color3f Material::emission(const Coord2f* tex_coord) const {
	return emission_;
}

GLMaterial Material::GenerateGLMaterial() {
	GLMaterial mat;

	Texture3u* texDiffuse = texture(Material::kDiffuseMapSlot);
	Texture3u* texRMA = texture(Material::kRMAMapSlot);
	Texture3u* texNormal = texture(Material::kNormalMapSlot);

	mat.normal = Color3f({ 0.f, 0.f, 1.f });
	mat.rma = Color3f({roughness_, metallicness, ior});
	mat.diffuse = Color3f({ 1.f, 1.f, 1.f });

	mat.texDiffuse = mat.texNormal = mat.texRMA = NULL;

	//Diffuse
	if (texDiffuse) {
		GLuint id = 0;
		CreateBindlessTexture(id, mat.texDiffuse, texDiffuse->width(), texDiffuse->height(), texDiffuse->data());
	}
	else {
		GLuint id = 0;
		GLubyte data[] = { 255,255,255,255 };
		CreateBindlessTexture(id, mat.texDiffuse, 1, 1, data);
		mat.diffuse = diffuse();
	}

	//RMA
	if (texRMA) {
		GLuint id = 0;
		CreateBindlessTexture(id, mat.texRMA, texRMA->width(), texRMA->height(), texRMA->data());
	}

	//Normal
	if (texNormal) {
		GLuint id = 0;
		CreateBindlessTexture(id, mat.texNormal, texNormal->width(), texNormal->height(), texNormal->data());
	}

	return mat;
}
#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__
#include "BaseComponent.h"
#include <MathGeoLib/include/Math/float2.h>

class CameraComponent;

struct Particle
{
	float3 Position;
	float3 Velocity;
	float LifeTime;
	bool IsAlive = false;
};

class ParticleEmitter : public BaseComponent
{
	DEFINE_COMPONENT(ParticleEmitter);
public:
	ParticleEmitter(int MaxParticles, float2 EmitArea, float FallHeight, float FallSpeed, float LifeTime);
	~ParticleEmitter();
	
	void Update(float dt) override;
	void EditorUpdate(float dt) override;

	void EndPlay() override;
	void CleanUp() override;

	void SetTexture(unsigned textureId);

	std::vector<Particle*> ParticlePool;
	float2 EmitArea;
	int MaxParticles;

	float FallHeight;
	float FallSpeed;
	float LifeTime;

private:
	void drawParticle(Particle* particle);
	void generateParticles();
	void checkValues();

	void ComputeQuad(const CameraComponent& camera, float3& up, float3& right, Particle* particle) const;

	float2 _controlEmitArea;
	int _controlMaxParticles;

	float _controlFallHeight;
	float _controlFallSpeed;
	float _controlLifeTime;

	unsigned _texture = 0;
	float _width, _height;

	bool _editorSimulation = false;

	std::shared_ptr<class ModuleCameraManager> _cameraManager;
};

#endif

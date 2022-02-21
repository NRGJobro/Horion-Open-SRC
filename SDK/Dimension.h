#pragma once
class HashedString;

class Dimension {
private:
	char pad_0x0[0x5A];  //0x0
public:
	__int16 maxHeight;  //0x5A
private:
	char pad_0x5E[0x5A];  //0x5E
public:
	class BrightnessRamp *brightnessRamp;  //0xB8
private:
	char pad_0xC0[0x20];  //0xC0
public:
	int dimensionId;  //0xE0
private:
	char pad_0xE4[0x1];  //0xE4
public:
	bool hasCeiling;  //0xE5
private:
	char pad_0xE6[0x2];  //0xE6
public:
	int timeThingy;  //0xE8
private:
	char pad_0xEC[0x34];  //0xEC
public:
	class Weather *weather;  //0x120
};

class Weather {
private:
	char pad_0x8[0x44];  //0x8
public:
	float fogLevel;  //0x4C

	virtual ~Weather();
	virtual void onSourceCreated(C_BlockSource &);
	virtual void onSourceDestroyed(C_BlockSource &);
	virtual void onAreaChanged(C_BlockSource &, vec3_ti const &, vec3_ti const &);
	virtual void onBlockChanged(C_BlockSource &, vec3_ti const &, unsigned int, C_Block const &, C_Block const &, int, class ActorBlockSyncMessage const *, class BlockChangedEventTarget);
	virtual void onBrightnessChanged(C_BlockSource &, vec3_ti const &);
	virtual void onBlockEntityChanged(C_BlockSource &, class BlockActor &);
	virtual void onBlockEntityAboutToBeRemoved(C_BlockSource &, std::shared_ptr<BlockActor>);
	virtual void onEntityChanged(C_BlockSource &, C_Entity &);
	virtual void onBlockEvent(C_BlockSource &, int, int, int, int, int);
	virtual void allChanged(void);
	virtual void addParticle(class ParticleType, vec3_t const &, vec3_t const &, int, CompoundTag const *, bool);
	virtual void sendServerLegacyParticle(ParticleType, vec3_t const &, vec3_t const &, int);
	virtual void addParticleEffect(HashedString const &, vec3_t const &, class MolangVariableMap const &);
	virtual void addParticleEffect2(HashedString const &, C_Entity const &, HashedString const &, vec3_t const &, MolangVariableMap const &);
	virtual void addTerrainParticleEffect(vec3_ti const &, C_Block const &, vec3_t const &, float, float, float);
	virtual void addTerrainSlideEffect(vec3_ti const &, C_Block const &, vec3_t const &, float, float, float);
	virtual void addBreakingItemParticleEffect(vec3_t const &, ParticleType, struct TextureUVCoordinateSet const &, bool);
	virtual void playMusic(std::string const &, vec3_t const &, float, float);
	virtual void playStreamingMusic(std::string const &, int, int, int);
	virtual void onEntityAdded(C_Entity &);
	virtual void onEntityRemoved(C_Entity &);
	virtual void onChunkLoaded(class ChunkSource &, class LevelChunk &);
	virtual void onChunkReloaded(ChunkSource &, LevelChunk &);
	virtual void onSubChunkLoaded(ChunkSource &, LevelChunk &, short);
	virtual void onChunkUnloaded(LevelChunk &);
	virtual void onLevelDestruction(std::string const &);
	virtual void levelEvent(class LevelEvent, vec3_t const &, int);
	virtual void levelEvent(LevelEvent, CompoundTag const &);
	virtual void levelSoundEvent(class LevelSoundEvent, vec3_t const &, int, class ActorDefinitionIdentifier const &, bool, bool);
	virtual void levelSoundEvent(std::string const &, vec3_t const &, float, float);
	virtual void stopSoundEvent(std::string const &);
	virtual void stopAllSounds(void);
	virtual void takePicture(__int64 &, C_Entity *, C_Entity *, struct ScreenshotOptions &);
	virtual void playerListChanged(void);
};
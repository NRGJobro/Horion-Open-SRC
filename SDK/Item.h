#pragma once
//#include "Tag.h"
#include <memory>

#include "../Utils/HMath.h"
#include "TextHolder.h"

class CompoundTag;
class Tag;
class BlockLegacy;
class BlockSource;
class Block;
class Entity;
class ItemStack;
class ScreenContext;
class ItemRenderer;
class ClientInstance;
class MinecraftGame;

class BaseActorRenderContext {
private:
	char pad_0x0[0x60];  //0x0000
public:
	ItemRenderer *renderer;  //0x0058
private:
	char pad_0x50[0x238];  //0x60
public:
	BaseActorRenderContext(ScreenContext *ScreenCtx, ClientInstance *client, MinecraftGame *game);
};

class ItemRenderer {
public:
	void renderGuiItemNew(BaseActorRenderContext *BaseActorRenderCtx, ItemStack *item, int mode, float x, float y, float opacity, float scale, bool isEnchanted);
	void renderGuiItemInChunk(BaseActorRenderContext *BaseActorRenderCtx, ItemStack *item, float x, float y, float opacity, float scale, bool isEnchanted, int mode);
};

class Player;
class ItemStack;
class Level;

class Item {
	char pad_0008[0x70];  //0x8
public:
	int16_t maxStackSize;  //0x78
	int16_t itemId;        //0x7A
private:
	char pad_0094[4];  //0x7C
public:
	TextHolder tileName;  //0x80
private:
	char pad_00B8[8];  //0xA0
public:
	TextHolder name;  //0xA8
private:
	char pad_00E0[8];  //0xC8
public:
	TextHolder isFrom;  //0xD0
private:
	char pad_0108[80];  //0xF0
public:
	int16_t duration;  //0x140

private:
	virtual void destructor();                     // 0
	virtual __int64 initServer(__int64, __int64);  // 1
	virtual __int64 tearDown(void);                // 2
public:
	virtual __int64 getMaxUseDuration(ItemStack const *);                                                                                // 3
	virtual __int64 getMaxUseDuration(ItemStack const *) const;                                                                          // 4
	virtual bool isMusicDisk(void);                                                                                                        // 5
	virtual __int64 executeEvent();                                                                                                        // 6
	virtual bool isComponentBased(void);                                                                                                   // 7
	virtual bool isArmor(void);                                                                                                            // 8
	virtual bool isBlockPlanterItem(void);                                                                                                 // 9
	virtual bool isBucket(void);                                                                                                           // 10
	virtual bool isCamera(void);                                                                                                           // 11
	virtual bool isCandle(void);                                                                                                           // 12
	virtual bool isDamageable(void);                                                                                                       // 13
	virtual bool isDyeable(void);                                                                                                          // 14
	virtual bool isDye(void);                                                                                                              // 15
	virtual __int64 getItemColor(void);                                                                                                    // 16
	virtual bool isFertilizer(void);                                                                                                       // 17
	virtual bool isFood(void);                                                                                                             // 18
	virtual bool isThrowable(void);                                                                                                        // 19
	virtual bool isUseable(void);                                                                                                          // 20
	virtual __int64 getComponent(TextHolder const &);                                                                                      // 21
	virtual __int64 getCamera(void);                                                                                                       // 22
	virtual __int64 getFood(void);                                                                                                         // 23
	virtual __int64 getFuel(void);                                                                                                         // 24
	virtual __int64 setMaxStackSize(unsigned char);                                                                                        // 25
	virtual __int64 setStackedByData(bool);                                                                                                // 26
	virtual __int64 setMaxDamage(int);                                                                                                     // 27
	virtual __int64 setHandEquipped(void);                                                                                                 // 28
	virtual __int64 setUseAnimation(int);                                                                                                  // 29
	virtual __int64 setMaxUseDuration(int);                                                                                                // 30
	virtual __int64 setRequiresWorldBuilder(bool);                                                                                         // 31
	virtual __int64 setExplodable(bool);                                                                                                   // 32
	virtual __int64 setFireResistant(bool);                                                                                                // 33
	virtual __int64 setIsGlint(bool);                                                                                                      // 34
	virtual __int64 setShouldDespawn(bool);												       // 35
	virtual void buildNetworkTag(void);//We forgored these funcs, the vtable index numbers are now invalid
    virtual void initializeFromNetwork(CompoundTag const &);
	virtual __int64 getBlockShape(void);                                                                                                   // 36
	virtual bool canBeDepleted(void);                                                                                                      // 37
	virtual bool canDestroySpecial(Block const &);                                                                                       // 38
	virtual __int64 getLevelDataForAuxValue(int);                                                                                          // 39
	virtual bool isStackedByData(void);                                                                                                    // 40
	virtual __int64 getMaxDamage(void);                                                                                                    // 41
	virtual __int64 getAttackDamage(void);                                                                                                 // 42
	virtual bool isHandEquipped(void);                                                                                                     // 43
	virtual bool isGlint(ItemStack const &);                                                                                             // 44
	virtual bool isPattern(void);                                                                                                          // 45
	virtual __int64 getPatternIndex(void);                                                                                                 // 46
	virtual __int64 showsDurabilityInCreative(void);                                                                                       // 47
	virtual bool isWearableThroughLootTable(CompoundTag const *);                                                                          // 48
	virtual bool canDestroyInCreative(void);                                                                                               // 49
	virtual bool isDestructive(int);                                                                                                       // 50
	virtual bool isLiquidClipItem(int);                                                                                                    // 51
	virtual __int64 shouldInteractionWithBlockBypassLiquid(Block const &);                                                               // 52
	virtual __int64 requiresInteract(void);                                                                                                // 53
	virtual __int64 appendFormattedHovertext(ItemStack const &, Level &, std::string &, bool);                                  // 54
	virtual bool isValidRepairItem();                                                                                                      // 55
	virtual __int64 getEnchantSlot(void);                                                                                                  // 56
	virtual __int64 getEnchantValue(void);                                                                                                 // 57
	virtual __int64 getArmorValue(void);                                                                                                   // 58
	virtual int getToughnessValue(void);
	virtual bool isComplex(void);                                                                                                          // 59
	virtual bool isValidAuxValue(int);                                                                                                     // 60
	virtual __int64 getDamageChance(int);                                                                                                  // 61
	virtual __int64 getViewDamping(void);                                                                                                  // 62
	virtual __int64 uniqueAuxValues(void);                                                                                                 // 63
	virtual bool isActorPlacerItem(void);                                                                                                  // 64
	virtual bool isMultiColorTinted(ItemStack const &);                                                                                  // 65
	virtual int getColor(CompoundTag const *, class ItemDescriptor const &);
	virtual bool hasCustomColor(ItemStack const &);                                                                                      // 67
	virtual bool hasCustomColor(CompoundTag const *);                                                                                      // 68
	virtual __int64 clearColor(ItemStack &);                                                                                             // 69
	virtual __int64 clearColor(CompoundTag *);                                                                                             // 70
	virtual __int64 setColor();                                                                                                            // 71
	virtual __int64 getBaseColor(ItemStack const &);                                                                                     // 72
	virtual __int64 getSecondaryColor(ItemStack const &);                                                                                // 73
	virtual __int64 getActorIdentifier(ItemStack const &);                                                                               // 74
	virtual __int64 buildIdAux(short, CompoundTag const *);                                                                                // 77
	virtual bool canUseOnSimTick(void);                                                                                                    // 78
	virtual __int64 use(ItemStack &, Player &);                                                                                        // 79
	virtual __int64 dispense(BlockSource &, __int64, int, Vec3 const &, unsigned char);                                                // 80
	virtual __int64 useTimeDepleted(ItemStack &, Level *, Player *);                                                          // 81
	virtual __int64 releaseUsing(ItemStack &, Player *, int);                                                                          // 82
	virtual __int64 getDestroySpeed(ItemStack const &, Block const &);                                                                 // 83
	virtual __int64 hurtActor(ItemStack &, Entity &, Entity &);                                                                      // 84
	virtual __int64 hitActor(ItemStack &, Entity &, Entity &);                                                                       // 85
	virtual __int64 hitBlock(ItemStack &, Block const &, Vec3i const &, Entity &);                                                 // 86
	virtual __int64 mineBlock(ItemStack &, Block const &, int, int, int, Entity *);                                                  // 87
	virtual __int64 mineBlock(ItemStack &, Block const &, int, int, int, Entity *) const;                                            // 88
	virtual __int64 buildDescriptionName(ItemStack const &);                                                                             // 89
	virtual __int64 buildDescriptionId(class ItemDescriptor const *, CompoundTag const *);                                                 // 90
	virtual __int64 buildEffectDescriptionName(ItemStack const &);                                                                       // 91
	virtual __int64 buildCategoryDescriptionName(void);                                                                                    // 92
	virtual __int64 readUserData(ItemStack &, __int64, __int64);                                                                         // 93
	virtual __int64 writeUserData(ItemStack const &, __int64);                                                                           // 94
	virtual __int64 getMaxStackSize(class ItemDescriptor const *);                                                                         // 95
	virtual __int64 inventoryTick(ItemStack &, Level &, Entity &, int, bool);                                                 // 96
	virtual __int64 refreshedInContainer(ItemStack const &, Level &);                                                           // 97
	virtual __int64 getCooldownType(void);                                                                                                 // 98
	virtual __int64 getCooldownTime(void);                                                                                                 // 99
	virtual __int64 fixupCommon(ItemStack &);                                                                                            // 100
	virtual __int64 fixupCommon(ItemStack &, Level &);                                                                          // 101
	virtual __int64 getDamageValue(CompoundTag const *);                                                                                   // 102
	virtual __int64 setDamageValue(ItemStack &, short);                                                                                  // 103
	virtual __int64 getInHandUpdateType(Player const &, ItemStack const &, ItemStack const &, bool, bool);                           // 104
	virtual __int64 getInHandUpdateType(Player const &, ItemStack const &, ItemStack const &, bool, bool, __int64 fakeuseotherone);  // 105
	virtual __int64 validFishInteraction(int);                                                                                             // 106
	virtual bool isSameItem(ItemStack const &, ItemStack const &);                                                                     // 107
	virtual __int64 initClient();                                                                                                          // 108
	virtual __int64 getInteractText(Player const &);                                                                                     // 109
	virtual __int64 getAnimationFrameFor(Entity *, bool, ItemStack const *, bool);                                                     // 110
	virtual bool isEmissive(int);                                                                                                          // 111
	virtual __int64 getLightEmission(int);                                                                                                 // 112
	virtual __int64 getIcon(ItemStack const &, int, bool);                                                                               // 113
	virtual __int64 getIconYOffset(void);                                                                                                  // 114
	virtual __int64 setIcon(const std::string &, int);                                                                                     // 115
	virtual __int64 setIcon();                                                                                                             // 116
	virtual __int64 setIconAtlas(const std::string &, int);                                                                                // 117
	virtual bool canBeCharged(void);                                                                                                       // 118
	virtual __int64 playSoundIncrementally(ItemStack const &, Entity &);                                                               // 119
	virtual __int64 playSoundIncrementally(ItemStack const &, Entity &) const;                                                         // 120
	virtual __int64 getFurnaceBurnIntervalMultipler(ItemStack const &);                                                                  // 121
	virtual __int64 getFurnaceXPmultiplier(ItemStack const &);                                                                           // 122
	virtual __int64 getAuxValuesDescription(void);                                                                                         // 123
	virtual __int64 _checkUseOnPermissions(Entity &, ItemStack &, unsigned char const &, Vec3i const &);                             // 124
	virtual __int64 _calculatePlacePos(ItemStack &, Entity &, unsigned char &, Vec3i &);                                             // 125
	virtual __int64 _useOn(ItemStack &, Entity &, Vec3i, unsigned char, float, float, float);                                        // 126

public:
	bool isTool(void) {
		if (getAttackDamage() > 0) return true;  // Does Attack Damage
		if (((std::string)name.getText()).find("flint_and_steel") != std::string::npos) return true;
		if (((std::string)name.getText()).find("compass") != std::string::npos) return true;
		if (((std::string)name.getText()).find("pearl") != std::string::npos) return true;
		if (((std::string)name.getText()).find("heart_of_the_sea") != std::string::npos) return true;
		if (((std::string)name.getText()).find("breath") != std::string::npos) return true;
		if (((std::string)name.getText()).find("paper") != std::string::npos) return true;
		if (((std::string)name.getText()).find("totem") != std::string::npos) return true;
		if (((std::string)name.getText()).find("emerald") != std::string::npos) return true;
		if (((std::string)name.getText()).find("diamond") != std::string::npos) return true;
		if (((std::string)name.getText()).find("gold") != std::string::npos) return true;
		if (((std::string)name.getText()).find("name_tag") != std::string::npos) return true;
		if (((std::string)name.getText()).find("enchanted_book") != std::string::npos) return true;
		if (((std::string)name.getText()).find("nautilus_shell") != std::string::npos) return true;
		return false;
	}
	bool isWeapon(void) {
		if (((std::string)name.getText()).find("sword") != std::string::npos) return true;
		if (((std::string)name.getText()).find("axe") != std::string::npos) return true;
		return false;
	}
	bool isShooter(void) {
		if (((std::string)name.getText()).find("bow") != std::string::npos) return true;
		if (((std::string)name.getText()).find("arrow") != std::string::npos) return true;
		return false;
	}
	bool isPickaxe(void) {
		if (((std::string)name.getText()).find("pickaxe") != std::string::npos) return true;
		return false;
	}
	bool isShovel(void) {
		if (((std::string)name.getText()).find("shovel") != std::string::npos) return true;
		return false;
	}

	bool isBlock(void) {
		auto val = *reinterpret_cast<__int64 ***>(reinterpret_cast<__int64>(this) + 0x1A0);
		return val != nullptr && *val != nullptr;
	}

	Item* setAllowOffhand(bool allow = true);
};

class ItemDescriptor {
private:
	char pad_0x0[0x98];  //0x0000
public:
	ItemDescriptor() {
		memset(this, 0x0, sizeof(ItemDescriptor));
	}
	ItemDescriptor(int id, int16_t itemData);
	void fromStack(ItemStack *item);
};

class ItemStack {
private:
	uintptr_t **vTable;  //0x0000
public:
	Item **item;     //0x08
	CompoundTag *tag;  //0x10
private:
	char pad_0x18[0xA];  //0x18
public:
	char count;  //0x22
private:
	char pad_0x1B[0x7D];  //0x23
public:
	ItemStack() {
		memset(this, 0x0, sizeof(ItemStack));
	}

	ItemStack(BlockLegacy &legacy, int count) {
		memset(this, 0x0, sizeof(ItemStack));
		reinit(legacy, count);
	}

	ItemStack(Item &item, int count, int itemData) {
		memset(this, 0x0, sizeof(ItemStack));
		reinit(item, count, itemData);
	}

	ItemStack(ItemStack const &src);

	ItemStack(Tag const &tag);

	void fromTag(Tag const &tag);

	void save(CompoundTag **tag);

	void setUserData(std::unique_ptr<Tag> tag);

	bool isValid() {
		return this->item != nullptr && *this->item != nullptr;
	}

	inline Item *getItem() {
		return *this->item;
	}

	void reinit(BlockLegacy &legacy, int count);

	void reinit(Item &item, int count, int itemData);

	int getEnchantValue(int enchantId);

	bool isEnchanted() {
		int enchantValue = 0;
		for (int i = 0; i < 40; i++) {
			enchantValue = this->getEnchantValue(i);
		}
		if (enchantValue != 0)
			return true;
		else
			return false;
	}

	float getArmorValueWithEnchants() {
		if (!this->item || !(*this->item)->isArmor())
			return 0;

		return (float)(((*this->item)->getArmorValue() + ((this->getEnchantValue(0) * 1.5f    // Protection
														   + this->getEnchantValue(5) * 0.4f  // Thorns
														   + this->getEnchantValue(3) * 0.4f  // Blast Protection
														   + this->getEnchantValue(1) * 0.4f  // Fire Protection
														   + this->getEnchantValue(4) * 0.4f  // Projectile Protection
														   ))));
	}

	float getAttackingDamageWithEnchants() {
		if (!this->item)
			return 0;
		int sharpnessValue = this->getEnchantValue(9);
		return (*this->item)->getAttackDamage() + 1.25f * sharpnessValue;
	}

	void setShowPickUp(bool b) {
		*(bool *)(reinterpret_cast<__int64>(this) + 0x30) = b;
	}

	void setLore(std::string lore);

private:
	inline void setVtable(void);
};

class ArmorItem : public Item {
private:
	char pad_0x108[0x228 - sizeof(Item)];  //0x100
public:
	int ArmorSlot;  //0x1B8

	bool isHelmet() {
		return ArmorSlot == 0;
	}
	bool isChestplate() {
		return ArmorSlot == 1;
	}
	bool isLeggings() {
		return ArmorSlot == 2;
	}
	bool isBoots() {
		return ArmorSlot == 3;
	}
};

class ItemRegistry {
public:
	static Item ***getItemFromId(void *ptr, int itemId);
	static Item ***lookUpByName(void *, void *, TextHolder &);
};

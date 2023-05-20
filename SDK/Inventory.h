#pragma once
#include "Item.h"
#include "../Utils/Utils.h"
class ItemStack;
class Inventory;
class Player;

class Inventory {
private:
	virtual ~Inventory();

public:
	bool isFull();
	void dropSlot(int slot);

	// DROPS WHOLE INVENTORY doesnt work tho
	void dropAll();

	virtual __int64 init();
	virtual bool serverInitItemStackIds(signed int a2, int a3, __int64 a4);
	virtual __int64 addContentChangeListener(__int64 a2);
	virtual __int64 removeContentChangeListener(__int64 a2);
	virtual ItemStack* getItemStack(int slot);
	virtual bool hasRoomForItem(ItemStack*);
	virtual __int64 addItem(ItemStack*);
	virtual __int64 addItemToFirstEmptySlot(ItemStack*);
	virtual __int64 setItem(int slot, const ItemStack&);
	virtual __int64 setItemWithForceBalance(int, const ItemStack&, bool);
	virtual __int64 removeItem(int, int);
	virtual __int64 removeAllItems();
	virtual __int64 dropContents(BlockSource&, const Vec3&, bool);
	virtual __int64 getContainerSize();
	virtual __int64 getMaxStackSize();
	virtual __int64 startOpen(Player&);
	virtual __int64 stopOpen(Player&);
	virtual __int64 getSlotCopies();
	virtual __int64 getSlots();
	virtual __int64 getItemCount(const ItemStack&);
	virtual __int64 findFirstSlotForItem(const ItemStack&);
	virtual __int64 canPushInItem();   // BlockSource &,int,int,ItemInstance const&
	virtual __int64 canPullOutItem();  // BlockSource &,int,int,ItemInstance const&
	virtual __int64 setContainerChanged(int);
	virtual __int64 setContainerMoved();
	virtual __int64 setCustomName(const TextHolder&);
	virtual __int64 hasCustomName();
	virtual __int64 readAdditionalSaveData();    //  CompoundTag const&
	virtual __int64 addAdditionalSaveData();     //  CompoundTag &
	virtual __int64 createTransactionContext();  // std::function<void(Inventory&, int, const ItemStack&, const ItemStack&)>, std::function<void(void)>
	virtual __int64 initializeContainerContents(BlockSource&);
	virtual bool isEmpty();
	virtual __int64 add(ItemStack&);
	virtual __int64 canAdd(const ItemStack&);
	//virtual void unknown();
	virtual __int64 clearSlot(int);
	virtual __int64 clearInventory(int);
	virtual __int64 load();  // ListTag const&,SemVersion const&,Level &
	//virtual void unknown2();
	virtual int getEmptySlotsCount();
	virtual int getFirstEmptySlot();
	virtual void setContainerSize();

	void moveItem(int from, int to);
	void swapSlots(int from, int to);

	bool isInContainerScreen() {
		switch (*reinterpret_cast<int*>(this + 0x20)) {
		case 3: {
			return false;
		} break;
		case 4: {
			return true;
		} break;
		}
		return false;
	}

	class TextHolder getPlayerName() {
		return *reinterpret_cast<class TextHolder*>(reinterpret_cast<__int64>(this) + 0x319);
	}
};

class Container {
public:
	class ItemStack* getItemStackFromSlot(int slot){
		return Utils::CallVFunc<5, class ItemStack*, int>(this, slot);
	}
};

class PlayerInventoryProxy {
private:
	char pad_0x0[0x10];  // 0x0
public:
	int selectedHotbarSlot;  // 0x10
private:
	char pad_0x14[0xAC];  // 0x14
public:
	Inventory* inventory;  // 0xC0
};

//Im not sure exactly where these unknown's go but the funcs we use work.

class ContainerScreenController {
private:
	virtual __int64 destructor();
	virtual __int64 tick(void);
	virtual __int64 handleEvent(__int64&);
	virtual __int64 getRoute(void);
	virtual __int64 setScreenState(__int64, std::string const&);
	virtual __int64 onOpen(void);
	virtual __int64 onTerminate(void);
	virtual __int64 onInit(void);
	virtual __int64 onDelete(void);
	virtual __int64 unknown1();

public:
	virtual bool canExit(void);
	virtual __int64 tryExit(void);

private:
	virtual __int64 areControllerTabsEnabled(void);
	virtual __int64 onCreation(void);
	virtual __int64 logCreationTime(std::string const&, double, double, unsigned char);
	virtual __int64 unknown2();
	virtual __int64 unknown3();

public:
	virtual __int64 onLeave(void);
	virtual __int64 leaveScreen(void);

private:
	virtual __int64 handleGameEventNotification(__int64);
	virtual __int64 bind(std::string const&, unsigned int, int, std::string const&, unsigned int, std::string const&, __int64&);
	virtual __int64 bind(std::string const&, unsigned int, std::string const&, __int64&);
	virtual __int64 handleLicenseChanged(void);
	virtual __int64 onDictationEvent(std::string const&);

public:
	virtual __int64 getCallbackInterval(void);
	virtual void setAssociatedBlockPos(Vec3i const&);
	virtual void setAssociatedEntityUniqueID(__int64);
	virtual void setSuspendInput(bool);
	virtual __int64 onRender(void);
	virtual __int64 addStaticScreenVars(__int64&);
	virtual __int64 getAdditionalScreenInfo(void);
	virtual __int64 getTelemetryOverride(void);
	virtual __int64 addEventProperties(__int64) const;
	virtual __int64 getSceneType(void);
	virtual __int64 getScreenVersion(void);
	virtual __int64 screenHandlesGamepadMenuButton(void);
	virtual __int64 getProxy(void);
	virtual __int64 onEntered(void);
	virtual __int64 getNameId(std::string const&);
	virtual __int64 _doesScreenHaveExitBehavior(void);
	virtual __int64 _isStillValid(void);
	virtual __int64 _getGamepadHelperVisible(void);
	virtual __int64 _getMixedHelperVisible(void);
	virtual __int64 _getKeyboardHelperVisible(void);
	virtual __int64 _getButtonADescription(void);
	virtual __int64 _getButtonBDescription(void);
	virtual __int64 _getButtonXDescription(void);
	virtual __int64 _getButtonYDescription(void);
	virtual __int64 _getButtonKeyboardDescription(void);

private:
	virtual void pleasePad1();
	virtual void pleasePad2();
	virtual void pleasePad3();

public:
	virtual __int64 _handlePlaceAll(std::string const&, int);
	virtual __int64 _handlePlaceOne(std::string const&, int);
	virtual __int64 _handleSelectSlot(std::string const&, int);
	virtual __int64 _getSelectedSlotInfo(void);
	virtual ItemStack* _getItemStack(TextHolder, int) const;

private:
	virtual __int64 _getVisualItemStack(std::string const&, int);
	virtual __int64 _getTakeableItemStackBase(std::string const&, int);
	virtual __int64 _onContainerSlotHovered(std::string const&, int);
	virtual __int64 _onContainerSlotSelected(std::string const&, int);
	virtual __int64 _onContainerSlotPressed(std::string const&, int);
	virtual __int64 _shouldSwap(std::string const&, int, std::string const&, int);
	virtual __int64 _getCollectionName(__int64*);
	virtual __int64 _canSplit(std::string const&, int);
	virtual __int64 _sendFlyingItem(__int64 const&, std::string const&, int, std::string const&, int);

public:
	void handleAutoPlace(std::string name, int slot);
};

class CraftingScreenController : public ContainerScreenController {
};

class ChestScreenController : public ContainerScreenController {
};
######################################################################
# QT Project file
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . . Source
INCLUDEPATH += . Source

# Input
HEADERS += $$(SRC_DIR)/AppConfig.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAppMain.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteBitfield.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteMainWindow.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteEditor.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteUVEditor.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteTimeline.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteProperties.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAnimationProperties.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteJointProperties.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteJointList.hpp  $$(SRC_DIR)/DT3SpriteEditor/EdSpriteLineEdit.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyBase.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAnimations.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyBoolField.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyDefaultField.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyBitfieldField.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAnimationTransferDialog.hpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteNewEventDialog.hpp

SOURCES += $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAppMain.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteBitfield.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteMainWindow.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteEditor.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteUVEditor.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteTimeline.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteProperties.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAnimationProperties.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteJointProperties.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteJointList.cpp   $$(SRC_DIR)/DT3SpriteEditor/EdSpriteLineEdit.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyBase.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAnimations.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyBoolField.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyDefaultField.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpritePropertyBitfieldField.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteAnimationTransferDialog.cpp $$(SRC_DIR)/DT3SpriteEditor/EdSpriteNewEventDialog.cpp

RESOURCES += $$(SRC_DIR)/DT3SpriteEditor/SpriteEditor.qrc
# Passive Acoustic Surveillance System

CC				:= gcc
PIO			:= pio
CFLAGS		:= -lm -g3 -lsqlite3 -ldsp -L./lib

SRC			:= ./src/*.h ./src/*.c
DEPENDS		:= gtk4 libadwaita-1 shumate-1.0
CONFIG		:= $(shell pkg-config --cflags --libs $(DEPENDS))
PROGRAM		:= SONAR

FIRMWARE		:= firmware.elf
FRM_DIR		:= ./firmware/ex/.pio/build/genericSTM32F446RE

OPENOCD		:= openocd 
INTERFACE	:= interface/stlink.cfg
TARGET		:= target/stm32f4x.cfg
COMMAND		:= "program $(FIRMWARE) verify reset exit"

.PHONY: firmware station firmware_remove

# Building and flashing the firmware
firmware:
	@echo "Building embedded firmware..."
	@cd ./firmware ; $(PIO) run ; cd ..
	@echo " "
	@echo "Uploading embedded firmware..."
	@cd $(FRM_DIR) ; openocd -f $(INTERFACE) -f $(TARGET) -c $(COMMAND)
	
# Building and running the ground station program
station:
	@echo "Building ground station..."
	$(CC) $(SRC) -o ./$(PROGRAM) $(CONFIG) $(CFLAGS)
	@echo " "
	@echo "Running ground station..."
	@./$(PROGRAM)

# Remove the old firmware
firmware_remove:
	@echo "Removing the old firmware..."
	@cd ./firmware ; $(PIO) run -t clean ; cd ..

include $(DEVKITPPC)/gamecube_rules
SOURCES := source
DATA := data
INCLUDES := 
LIBS := -lmad -logc -lm

all: $(DATA)/$(TARGET).dol

$(DATA)/$(TARGET).dol: $(DATA)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(DATA)/$(TARGET).elf: $(SOURCES)/$(TARGET).o
	$(LD) -o $@ $< $(LIBS)

$(SOURCES)/$(TARGET).o: $(SOURCES)/$(TARGET).c
	$(CC) -c -o $@ $< $(INCLUDES)

clean:
	rm -f $(SOURCES)/$(TARGET).o $(DATA)/$(TARGET).elf $(DATA)/$(TARGET).dol
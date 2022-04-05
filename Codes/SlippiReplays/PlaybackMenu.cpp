#include "PlaybackMenu.h"

namespace replayMenus {
    Replays* replaysObj;
    SIMPLE_INJECTION(pullReplayStrings, 0x8119795c, "stwu sp, -0x0020(sp)") {
        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  DISPLAY REPLAY FILES  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();
        EXIPacket getNumberElementsPacket = EXIPacket(GET_NUMBER_REPLAY_FILES, nullptr, 0);
        getNumberElementsPacket.Send();

        u8 cmd_byte = 0;
        do {
            // read in one byte from emulator to see how to deal with the rest of the exi buffer
            readEXI(&cmd_byte, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
        } while(cmd_byte != NUM_REPLAYS);
        

        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  GET THE NUMBER OF REPLAYS  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        u32 numElements = 0;
        readEXI(&numElements, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

        if(numElements = 0){
            _OSDisableInterrupts();
            OSReport("  ~~~~~~~~~~~~~~~~ NO REPLAYS FOUND ~~~~~~~~~~~~~~~~  \n");
            _OSEnableInterrupts();
            //need to actually handle 0 by initializing a zero replaysObj I guess
            return;
        }

        EXIPacket getReplaysSizePacket = EXIPacket(GET_REPLAY_FILES_SIZE, nullptr, 0);
        getReplaysSizePacket.Send();

        do {
            // read in one byte from emulator to see how to deal with the rest of the exi buffer
            readEXI(&cmd_byte, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
        } while(cmd_byte != REPLAY_FILES_SIZE);

        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  GET THE SIZE OF REPLAYS  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        vector<u8> sizeOfElements(numElements, 0);
        readEXI(sizeOfElements.getPtr(), numElements, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

        EXIPacket getReplaysPacket = EXIPacket(GET_REPLAY_FILES, nullptr, 0);
        getReplaysPacket.Send();
        do {
            // read in one byte from emulator to see how to deal with the rest of the exi buffer
            readEXI(&cmd_byte, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
        } while(cmd_byte != REPLAYS);

        u32 read_data_size = 0;
        for( size_t i = 0; i < sizeOfElements.size(); i++){
            read_data_size += sizeOfElements[i];
        }

        vector<u8> dataVec(read_data_size, 0);
        readEXI(dataVec.getPtr(), read_data_size, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  GET THE REPLAYS  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  SETUP REPLAYS OBJECT  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        EXIPacket getSizeOfNamesPacket = EXIPacket(GET_REPLAY_NAMES_SIZE, nullptr, 0);
        getSizeOfNamesPacket.Send();

        do {
            // read in one byte from emulator to see how to deal with the rest of the exi buffer
            readEXI(&cmd_byte, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
        } while(cmd_byte != REPLAY_NAMES_SIZE);

        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  GET THE SIZE OF REPLAY NAMES  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  SETTING SIZE OF NAMES ARRAY  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        vector<u8> sizeOfNames(numElements, 0);
        readEXI(sizeOfNames.getPtr(), numElements, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

        replaysObj = new Replays(numElements, sizeOfElements.getPtr(), sizeOfNames.getPtr());

        //why was dataVec u8 type if we are casting to char here?
        replaysObj->SetReplays((char**)dataVec.getPtr());

        EXIPacket getReplayNamesPacket = EXIPacket(GET_REPLAY_NAMES, nullptr, 0);
        getReplayNamesPacket.Send();
        do {
            // read in one byte from emulator to see how to deal with the rest of the exi buffer
            readEXI(&cmd_byte, 1, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);
        } while(cmd_byte != REPLAY_NAMES);

        
        u32 name_data_size = 0;
        for( size_t i = 0; i < sizeOfNames.size(); i++){
            name_data_size += sizeOfNames[i];
        }

        vector<u8> dataNameVec(name_data_size, 0);
        readEXI(dataNameVec.getPtr(), name_data_size, EXIChannel::slotB, EXIDevice::device0, EXIFrequency::EXI_32MHz);

        _OSDisableInterrupts();
        OSReport("  ~~~~~~~~~~~~~~~~  GET THE NAMES OF THE REPLAYS  ~~~~~~~~~~~~~~~~  \n");
        _OSEnableInterrupts();

        replaysObj->SetNames((char**)dataNameVec.getPtr());
    }

    SIMPLE_INJECTION(overwriteMenu, 0x811979c8, "li r5, 5") {

    }
}
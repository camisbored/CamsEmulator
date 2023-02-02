			int i;
			char keyCheck = 0;

void runOp(HWND hwnd){
	lastOp = lastRead;
	switch(lastRead){
		case 0x00:
			isRunning = 0;
			break;
		case 0x01:
			r1 = readByte();
			break;
		case 0x02:
			r2 = readByte();
			break;
		case 0x03:
			r3 = readByte();
			break;
		case 0x04:
			r4 = readByte();
			break;
		case 0x05:
			r5 = readByte();
			break;
		case 0x06:
			r1 = ramMemory[readByte()];
			break;
		case 0x07:
			r2 = ramMemory[readByte()];
			break;
		case 0x08:
			r3 = ramMemory[readByte()];
			break;
		case 0x09:
			r4 = ramMemory[readByte()];
			break;
		case 0x0A:
			r5 = ramMemory[readByte()];
			break;
		case 0x0B:
			ramMemory[readByte()]=r1;
			break;
		case 0x0C:
			ramMemory[readByte()]=r2;
			break;
		case 0x0D:
			ramMemory[readByte()]=r3;
			break;
		case 0x0E:
			ramMemory[readByte()]=r4;
			break;
		case 0x0F:
			ramMemory[readByte()]=r5;
			break;
		case 0x10:
			r1 = r1+readByte();
			break;
		case 0x11:
			r2 = r2+readByte();
			break;
		case 0x12:
			r3 = r3+readByte();
			break;
		case 0x13:
			r4 = r4+readByte();
			break;
		case 0x14:
			r5 = r5+readByte();
			break;
		case 0x15:
			r1 = r1-readByte();
			break;
		case 0x16:
			r2 = r2-readByte();
			break;
		case 0x17:
			r3 = r3-readByte();
			break;
		case 0x18:
			r4 = r4-readByte();
			break;
		case 0x19:
			r5 = r5-readByte();
			break;
		case 0x1A:
			//reserved for future use
			break;
		case 0x1B:
			r1 = r1*readByte();
			break;
		case 0x1C:
			r2 = r2*readByte();
			break;
		case 0x1D:
			r3 = r3*readByte();
			break;
		case 0x1E:
			r4 = r4*readByte();
			break;
		case 0x1F:
			r5 = r5*readByte();
			break;
		case 0x20:
			r1 = r1/readByte();
			break;
		case 0x21:
			r2 = r2/readByte();
			break;
		case 0x22:
			r3 = r3/readByte();
			break;
		case 0x23:
			r4 = r4/readByte();
			break;
		case 0x24:
			r5 = r5/readByte();
			break;
		case 0x25:
			push(r1);
			break;
		case 0x26:
			push(r2);
			break;
		case 0x27:
			push(r3);
			break;
		case 0x28:
			push(r4);
			break;
		case 0x29:
			push(r5);
			break;
		case 0x2A:
			pop(0x01);
			break;
		case 0x2B:
			pop(0x02);
			break;
		case 0x2C:
			pop(0x03);
			break;
		case 0x2D:
			pop(0x04);
			break;
		case 0x2E:
			pop(0x05);
			break;
		case 0x2F:
			flag = (r1==readByte());
			break;
		case 0x30:
			flag = (r2==readByte());
			break;
		case 0x31:
			flag = (r3==readByte());
			break;
		case 0x32:
			flag = (r4==readByte());
			break;
		case 0x33:
			flag = (r5==readByte());
			break;
		case 0x34:
			r1++;
			break;
		case 0x35:
			r2++;
			break;
		case 0x36:
			r3++;
			break;
		case 0x37:
			r4++;
			break;
		case 0x38:
			r5++;
			break;
		case 0x39:
			r1--;
			break;
		case 0x3A:
			r2--;
			break;
		case 0x3B:
			r3--;
			break;
		case 0x3C:
			r4--;
			break;
		case 0x3D:
			r5--;
			break;
		case 0x3E:
            //reserved for future use
			break;
		case 0x3F:
			r1 = r1 << readByte();
			break;
		case 0x40:
			r2 = r2 << readByte();
			break;
		case 0x41:
			r3 = r3 << readByte();
			break;
		case 0x42:
			r4 = r4 << readByte();
			break;
		case 0x43:
			r5 = r5 << readByte();
			break;
		case 0x44:
			r1 = r1 >> readByte();
			break;
		case 0x45:
			r2 = r2 >> readByte();
			break;
		case 0x46:
			r3 = r3 >> readByte();
			break;
		case 0x47:
			r4 = r4 >> readByte();
			break;
		case 0x48:
			r5 = r5 >> readByte();
			break;
		case 0x49:
			r1 = r1 & readByte();
			break;
		case 0x4A:
			r2 = r2 & readByte();
			break;
		case 0x4B:
			r3 = r3 & readByte();
			break;
		case 0x4C:
			r4 = r4 & readByte();
			break;
		case 0x4D:
			r5 = r5 & readByte();
			break;
		case 0x4E:
			r1 = r1 | readByte();
			break;
		case 0x4F:
			r2 = r2 | readByte();
			break;
		case 0x50:
			r3 = r3 | readByte();
			break;
		case 0x51:
			r4 = r4 | readByte();
			break;
		case 0x52:
			r5 = r5 | readByte();
			break;
		case 0x53:
			r1 = r1 ^ readByte();
			break;
		case 0x54:
			r2 = r2 ^ readByte();
			break;
		case 0x55:
			r3 = r3 ^ readByte();
			break;
		case 0x56:
			r4 = r4 ^ readByte();
			break;
		case 0x57:
			r5 = r5 ^ readByte();
			break;
		case 0x58:
			r1 = ~r1;
			break;
		case 0x59:
			r2 = ~r2;
			break;
		case 0x5A:
			r3 = ~r3;
			break;
		case 0x5B:
			r4 = ~r4;
			break;
		case 0x5C:
			r5 = ~r5;
			break;
		case 0x5D:
			stack[0]=stack[0]+r1;
			break;
		case 0x5E:
			stack[0]=stack[0]+r2;
			break;
		case 0x5F:
			stack[0]=stack[0]+r3;
			break;
		case 0x60:
			stack[0]=stack[0]+r4;
			break;
		case 0x61:
			stack[0]=stack[0]+r5;
			break;
		case 0x62:
			stack[0]=stack[0]-r1;
			break;
		case 0x63:
			stack[0]=stack[0]-r2;
			break;
		case 0x64:
			stack[0]=stack[0]-r3;
			break;
		case 0x65:
			stack[0]=stack[0]-r4;
			break;
		case 0x66:
			stack[0]=stack[0]-r5;
			break;
		case 0x67:
			stack[0]=stack[0]*r1;
			break;
		case 0x68:
			stack[0]=stack[0]*r2;
			break;
		case 0x69:
			stack[0]=stack[0]*r3;
			break;
		case 0x6A:
			stack[0]=stack[0]*r4;
			break;
		case 0x6B:
			stack[0]=stack[0]*r5;
			break;
		case 0x6C:
			stack[0]=stack[0]/r1;
			break;
		case 0x6D:
			stack[0]=stack[0]/r2;
			break;
		case 0x6E:
			stack[0]=stack[0]/r3;
			break;
		case 0x6F:
			stack[0]=stack[0]/r4;
			break;
		case 0x70:
			stack[0]=stack[0]/r5;
			break;
		case 0x71:
			flag = (stack[0]==r1);
			break;
		case 0x72:
			flag = (stack[0]==r2);
			break;
		case 0x73:
			flag = (stack[0]==r3);
			break;
		case 0x74:
			flag = (stack[0]==r4);
			break;
		case 0x75:
			flag = (stack[0]==r5);
			break;
		case 0x76:
			stack[0]=stack[0]<<r1;
			break;
		case 0x77:
			stack[0]=stack[0]<<r2;
			break;
		case 0x78:
			stack[0]=stack[0]<<r3;
			break;
		case 0x79:
			stack[0]=stack[0]<<r4;
			break;
		case 0x7A:
			stack[0]=stack[0]<<r5;
			break;
		case 0x7B:
			stack[0]=stack[0]>>r1;
			break;
		case 0x7C:
			stack[0]=stack[0]>>r2;
			break;
		case 0x7D:
			stack[0]=stack[0]>>r3;
			break;
		case 0x7E:
			stack[0]=stack[0]>>r4;
			break;
		case 0x7F:
			stack[0]=stack[0]>>r5;
			break;
		case 0x80:
			stack[0]=stack[0] & r1;
			break;
		case 0x81:
			stack[0]=stack[0] & r2;
			break;
		case 0x82:
			stack[0]=stack[0] & r3;
			break;
		case 0x83:
			stack[0]=stack[0] & r4;
			break;
		case 0x84:
			stack[0]=stack[0] & r5;
			break;
		case 0x85:
			stack[0]=stack[0] | r1;
			break;
		case 0x86:
			stack[0]=stack[0] | r2;
			break;
		case 0x87:
			stack[0]=stack[0] | r3;
			break;
		case 0x88:
			stack[0]=stack[0] | r4;
			break;
		case 0x89:
			stack[0]=stack[0] | r5;
			break;
		case 0x8A:
			stack[0]=stack[0] ^ r1;
			break;
		case 0x8B:
			stack[0]=stack[0] ^ r2;
			break;
		case 0x8C:
			stack[0]=stack[0] ^ r3;
			break;
		case 0x8D:
			stack[0]=stack[0] ^ r4;
			break;
		case 0x8E:
			stack[0]=stack[0] ^ r5;
			break;
		case 0x8F:
			push(r1);
			push(r2);
			push(r3);
			push(r4);
			push(r5);
			break;
		case 0x90:
			pop(0x05);
			pop(0x04);
			pop(0x03);
			pop(0x02);
			pop(0x01);
			break;
		case 0x91:
			ip = readByte();
			break;
		case 0x92:
			if (flag)
				ip = readByte();
            else 
                readByte();
			break;
		case 0x93:
			if (!flag)
				ip = readByte();
            else 
                readByte();
			break;
		case 0x94:
			push(ip+1);
			ip = readByte();
			break;
		case 0x95:
			ip = stack[0];
			pop(-1);
			break;
		case 0x96:
			ip = stack[0];
			break;
		case 0x97:
			keyWrapper();
			r5 = lastPressed;
			break;
		case 0x98:
		for(i = 8; i <= 190; i++) {
	        if (GetAsyncKeyState(i) == -32767){
				lastPressed = i;
				keyCheck = 1;
				//break;
			}	
	    }
		if (keyCheck == 1){
			r5 = lastPressed;
		}
		keyCheck = 0;
			// Sleep(100);
			// checkKeyWrapper();
			//  if (keyPressedForCheck){
			// 	r5=lastPressed;
			// 	printf("%d\n", r5);
			//  }
			break;
		case 0x99:
			SetPixel(hdcMem, r1, r2, RGB(r3, r4, r5));
			break;
		case 0x9A:
			SetPixel(hdcMem, r1, r2, RGB(r3, r4, r5));
			redraw(hwnd);
			break;
		case 0x9B:
			redraw(hwnd);
			break;
		case 0x9C:
			r5 = rand()%255;
			break;
		case 0x9D:
			//Beep(50*readByte(), 50*readByte());
			beepAsyncWrapper(); readByte(); readByte();
			break;
		case 0x9E:
			Sleep(1*readByte());
			break;
		case 0x9F:
			flag = 0;
			break;
		case 0xA0:
			flag = 1;
			break;
		case 0xA1:
			//NO-OP
			break;
		default:
			isRunning = 0;
			sprintf(errorMessage, "Illegal instruction detected: 0x%2X", lastRead);
			MessageBox(0,errorMessage,"Runtime Error",MB_OK); 	
	}	
}
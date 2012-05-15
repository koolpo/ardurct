package com.google.code.ardurct.libraries;

import com.google.code.ardurct.hardware.XBeeHardware;


public class XBee extends Wiring {
	
	public static final String XBEE_COMMAND_SEQUENCE = "+++";
	
	// Commands
	private static final int XBEE_CMD_NONE = 0;
	private static final int XBEE_CMD_ENTER_CMD_MODE = '+';
	private static final int XBEE_CMD_EXIT_CMD_MODE = '-';
	private static final int XBEE_CMD_COMMIT = 'W';
	private static final int XBEE_CMD_ID_WRITE = 'I';
	private static final int XBEE_CMD_ID_READ = 'i';
	private static final int XBEE_CMD_BAUDRATE_WRITE = 'B';
	private static final int XBEE_CMD_SH_READ = 's';
	private static final int XBEE_CMD_SL_READ = 't';
	private static final int XBEE_CMD_DHDL_WRITE = 'D';
	private static final int XBEE_CMD_DH_READ = 'd';
	private static final int XBEE_CMD_DL_READ = 'e';
	private static final int XBEE_CMD_GUARD_WRITE = 'G';
	private static final int XBEE_CMD_RADIO_DB_READ = 'r';
	private static final int XBEE_CMD_ENABLE_64B_ADRESSING = 'A';

	// States
	private static final int XBEE_IDLE = 0;
	private static final int XBEE_GUARDING_BEFORE = 1;
	private static final int XBEE_GUARDING_AFTER = 2;
	private static final int XBEE_WAIT_FOR_OK = 3;
	private static final int XBEE_SEND_COMMAND = 4;
	private static final int XBEE_WRITING = 5;
	private static final int XBEE_PROCESSING = 6;

	// Validity
	private static final int XBEE_VALID_ID = (0x01 << 0);
	private static final int XBEE_VALID_SHSL =(0x01 << 1);
	private static final int XBEE_VALID_DHDL = (0x01 << 2);
	private static final int XBEE_VALID_RADIO_DB = (0x01 << 3);

	
	public HardwareSerial hardware;
	private int _state;
	private int _command;
	private int _valid;
	private int _guardTime;
	private int _baudrate;
	private long _waitUntilTime;
	private int _temp;
	private int[] _id = new int[4];
	private int[] _shsl = new int[16];
	private int[] _dhdl = new int[16];
	private int[] _buffer = new int[12];
	private int _counter;
	private int _radioDb;
	private boolean _isInCommandMode;

	
	public XBee() {	
	    _state = XBEE_IDLE;
	    _command = XBEE_CMD_NONE;
		_valid = 0;
		_isInCommandMode = false;
	}
	
	public void begin(HardwareSerial serial, int baudrate, int guardTime) {
		serial.begin(baudrate);
		hardware = serial;   
		_guardTime = guardTime;
		_baudrate = baudrate;
	}

	public void begin(HardwareSerial serial, int baudrate) {
		begin(serial, baudrate, XBeeHardware.XBEE_DEFAULT_GUARD_TIME);
	}

	public void begin(HardwareSerial serial) {
		begin(serial, XBeeHardware.XBEE_DEFAULT_BAUDRATE, XBeeHardware.XBEE_DEFAULT_GUARD_TIME);
	}

	public int available() {
		return hardware.availableFromReceivedQueue();
	}
	
	public int read() {
		return hardware.readFromReceivedQueue();
	}
	
	public void print(String s) {
		hardware.inboundQueue.append(s);
	}
	
	public boolean setId(int[] id) {
		if (!_isInCommandMode) return false;
		int i;
		// check if id is the same
		for (i=0; i<4; i++) {
			if (_id[i] != id[i]) break;
		}
		if (i == 4) return true;
		// if busy in another command, return
		if (isProcessingCommand()) return false;
	    for (i=0; i<4; i++) _id[i] = id[i];
	    _command = XBEE_CMD_ID_WRITE;
		return true;
	}


	public int[] getId() {
		if ((_valid & XBEE_VALID_ID) != 0) return _id;
		if (!_isInCommandMode) return null;
		if (isProcessingCommand()) return null;
		 _command = XBEE_CMD_ID_READ;
		return null;
	}


	public boolean setDHDL(int[] dhdl) {
		if (!_isInCommandMode) return false;
		int i;
		// check if dhdl is the same
		for (i=0; i<16; i++) {
			if (_dhdl[i] != dhdl[i]) break;
		}
		if (i == 16) return true;
		// if busy in another command, return
		if (isProcessingCommand()) return false;
	    for (i=0; i<16; i++) _dhdl[i] = dhdl[i];
	    _command = XBEE_CMD_DHDL_WRITE;
		return true;
	}

	public int[] getDHDL() {
		if ((_valid & XBEE_VALID_DHDL) != 0) return _dhdl;
		if (!_isInCommandMode) return null;
		if (isProcessingCommand()) return null;
		 _command = XBEE_CMD_DH_READ;
		return null;
	}

	public int[] getSHSL() {
		if ((_valid & XBEE_VALID_SHSL) != 0) return _shsl;
		if (!_isInCommandMode) return null;
		if (isProcessingCommand()) return null;
		 _command = XBEE_CMD_SH_READ;
		return null;
	}

	public int getRadioDb() {
		if ((_valid & XBEE_VALID_RADIO_DB) != 0) {
			// only read once
			_valid &= ~XBEE_VALID_RADIO_DB;
			return _radioDb;
		}
		if (!_isInCommandMode) return -1;
		if (isProcessingCommand()) return -1;
		 _command = XBEE_CMD_RADIO_DB_READ;
		return -1;
	}


	public boolean enterCommandMode() {
		if (_isInCommandMode) return true;
		if (isProcessingCommand()) return false;
		_command = XBEE_CMD_ENTER_CMD_MODE;
		return true;
	}

	public boolean exitCommandMode() {
		if (!_isInCommandMode) return true;
		if (isProcessingCommand()) return false;
		_command = XBEE_CMD_EXIT_CMD_MODE;
		return true;
	}

	public boolean commitToXBeeFlash() {
		if (isProcessingCommand()) return false;
		_command = XBEE_CMD_COMMIT;
		return true;
	}
	
	public boolean setBaudrate(int baudrate) {
		if (baudrate == _baudrate) return true;
		if (!_isInCommandMode) return false;
		if (isProcessingCommand()) return false;
		_baudrate = baudrate;
		_command = XBEE_CMD_BAUDRATE_WRITE;
		return true;
	}


	public boolean setGuardTime(int guardTime) {
		if (guardTime == _guardTime) return true;
		if (!_isInCommandMode) return false;
		if (isProcessingCommand()) return false;
		_temp = guardTime;
		_command = XBEE_CMD_GUARD_WRITE;
		return true;
	}


	public boolean enable64BitsAdressing() {
		if (!_isInCommandMode) return false;
		if (isProcessingCommand()) return false;
		_command = XBEE_CMD_ENABLE_64B_ADRESSING;
		return true;
	}

			
	public boolean isProcessingCommand() {
	    return (_command != XBEE_CMD_NONE);
	}

	public boolean isInCommandMode() {
		return _isInCommandMode;
	}

	public void processCommand() {
	    // If nothing to do return
	    if (_command == XBEE_CMD_NONE) return;
	    
	    switch (_state) {
	        case XBEE_IDLE:
	        	if (!_isInCommandMode) {
	    			_waitUntilTime = millis() + _guardTime;
	    			_state = XBEE_GUARDING_BEFORE;
	    		} else _state = XBEE_SEND_COMMAND;
	            break;
	            
	        case XBEE_GUARDING_BEFORE:
	            // wait for the guard time to elapse
	            if ((long)(millis()-_waitUntilTime) < 0) return;
	            // enter command mode
	            print(XBEE_COMMAND_SEQUENCE);
	            // prepare to wait
	            _waitUntilTime = millis() + _guardTime;
	            _state = XBEE_GUARDING_AFTER;
	            break;
	            
	        case XBEE_GUARDING_AFTER:
	            // wait for the guard time to elapse
	            if ((long)(millis()-_waitUntilTime) < 0) return;
				_state = XBEE_WAIT_FOR_OK;
				_counter = 0;

			case XBEE_WAIT_FOR_OK:
				while (available() > 0) {
					_buffer[_counter++] = read();
					if ((_buffer[_counter-1] == '\n') || (_counter > 9)) break;
				}
				// we have the end of line
				if ((_counter > 0) && (_buffer[_counter-1] == '\n'))  {
					// check for OK
					if ((_counter > 2) && (_buffer[_counter-3] == 'O') && (_buffer[_counter-2] == 'K')) _isInCommandMode = true;
				}	
				_command = XBEE_CMD_NONE;
				_state = XBEE_IDLE;
				break;

			case XBEE_SEND_COMMAND:
				switch (_command) {
	           		case XBEE_CMD_COMMIT:
	           			print("ATWR\n");
	           			break;
	           		case XBEE_CMD_EXIT_CMD_MODE:
	           			print("ATCN\n");
	           			break;
	                case XBEE_CMD_ID_READ:
	                    print("ATID\n");
	                    break;
	                case XBEE_CMD_ID_WRITE:
	                    print("ATID");
	                    for (int i=0; i<4; i++) print((char)_id[i]);
	                    print("\n");
	                    break;
	                case XBEE_CMD_SH_READ:
	                    print("ATSH\n");
	                    break;
	                case XBEE_CMD_SL_READ:
	                    print("ATSL\n");
	                    break;
	                case XBEE_CMD_DH_READ:
	                    print("ATDH\n");
	                    break;
	                case XBEE_CMD_DL_READ:
	                    print("ATDL\n");
	                    break;
	                case XBEE_CMD_DHDL_WRITE:
	                    print("ATDH");
						for (int i=0; i<8; i++) print((char)_dhdl[i]);
	                    print(",DL");
						for (int i=8; i<16; i++) print((char)_dhdl[i]);
						print("\n");
	                    break;
	                case XBEE_CMD_BAUDRATE_WRITE:
	                    print("ATBD");
	                    print(_baudrate, HEX);
	                    print("\n");
	                    break;
	                case XBEE_CMD_GUARD_WRITE:
	                    print("ATGT");
	                    print(_temp, HEX);
	                    print("\n");
	                    break;
	                case XBEE_CMD_RADIO_DB_READ:
	                    print("ATDB\n");
	                    break;
					case XBEE_CMD_ENABLE_64B_ADRESSING:
						print("ATMYFFFF\n");
						break;
	                default:
	                    break;
	            }
	           	_state = XBEE_PROCESSING;
	           	if (_command == XBEE_CMD_COMMIT) {
	           		_waitUntilTime = millis() + 100;
					_state = XBEE_WRITING;
	           	}
				_counter = 0;
	            break;
	            
	        case XBEE_WRITING:
	            // wait for the guard time to elapse
	            if ((long)(millis()-_waitUntilTime) < 0) return;
				// indicate to the hardware to communicate at this baudrate from now on
				_state = XBEE_PROCESSING;
				
			case XBEE_PROCESSING:
				if (_command == XBEE_CMD_EXIT_CMD_MODE) _isInCommandMode = false;
				else if (_command == XBEE_CMD_BAUDRATE_WRITE) hardware.begin(_baudrate);
				else if (_command == XBEE_CMD_ID_WRITE) _valid |= XBEE_VALID_ID;
				else if (_command == XBEE_CMD_DHDL_WRITE) _valid |= XBEE_VALID_DHDL;
				else if (_command == XBEE_CMD_GUARD_WRITE) _guardTime = _temp;
				while (available() > 0) {
					_buffer[_counter++] = read();
					if ((_buffer[_counter-1] == '\n') || (_counter > 9)) break;
				}
				// we have the end of line
				if ((_counter > 0) && (_buffer[_counter-1] == '\n'))  {
					_counter--; 	// remove the \n
					switch (_command) {
						case XBEE_CMD_ID_READ:
							for (int i=0; i<4; i++) _id[i] = '0';
							for (int i=0; i<_counter; i++) _id[3-i] = _buffer[_counter-1-i];
							_valid |= XBEE_VALID_ID;
							_command = XBEE_CMD_NONE;
							break;
						case XBEE_CMD_DH_READ:
							for (int i=0; i<8; i++) _dhdl[i] = '0';
							for (int i=0; i<_counter; i++) _dhdl[7-i] = _buffer[_counter-1-i];
							_command = XBEE_CMD_DL_READ;
							_state = XBEE_SEND_COMMAND;
							break;
						case XBEE_CMD_DL_READ:
							for (int i=0; i<8; i++) _dhdl[i+8] = '0';
							for (int i=0; i<_counter; i++) _dhdl[7-i+8] = _buffer[_counter-1-i];
							_valid |= XBEE_VALID_DHDL;
							_command = XBEE_CMD_NONE;
							break;
						case XBEE_CMD_SH_READ:
							for (int i=0; i<8; i++) _shsl[i] = '0';
							for (int i=0; i<_counter; i++) _shsl[7-i] = _buffer[_counter-1-i];
							_command = XBEE_CMD_SL_READ;
							_state = XBEE_SEND_COMMAND;
							break;
						case XBEE_CMD_SL_READ:
							for (int i=0; i<8; i++) _shsl[i+8] = '0';
							for (int i=0; i<_counter; i++) _shsl[7-i+8] = _buffer[_counter-1-i];
							_valid |= XBEE_VALID_SHSL;
							_command = XBEE_CMD_NONE;
							break;
						case XBEE_CMD_RADIO_DB_READ:
							if (_buffer[0] < 'A') _radioDb = (_buffer[0]-'0') << 4;
							else _radioDb = (_buffer[0]-'A'+10) << 4;
							if (_buffer[1] < 'A') _radioDb += _buffer[1]-'0';
							else _radioDb += _buffer[1]-'A'+10;
							_valid |= XBEE_VALID_RADIO_DB; 
						default:
							_command = XBEE_CMD_NONE;
							break;
					}
				}
				// there was an error, we return to no command
				if ((_state == XBEE_PROCESSING) && (_counter > 9)) _command = XBEE_CMD_NONE;
				// if no command, return to IDLE state
				if (_command == XBEE_CMD_NONE) _state = XBEE_IDLE;
	            break;
		}
	}
	
	public void print(byte[] s) {
		print(new String(s, 0, s.length));
	}

	public void print(int[] s) {
		byte bS[] = new byte[s.length];
		for (int i=0; i<s.length; i++) bS[i] = (byte)s[i];
		print(new String(bS, 0, s.length));
	}

	public void print(int n) {
		print(n+"");
	}

	public void print(char c) {
		print("" + ((char)c));
	}

	public void print(long n) {
		print(n+"");
	}

	public void print(int n, int radix) {
		print(Integer.toString(n, radix) + "");
	}

	public void print(long n, int radix) {
		print(Long.toString(n, radix) + "");
	}

	public void println(String s) {
		print(s + "\n");
	}

	public void println(byte[] s) {
		print(s);
		print("\n");
	}

	public void println(int[] s) {
		print(s);
		print("\n");
	}

	
	public void println(int n) {
		print(n + "\n");
	}

	public void println(long n) {
		print(n + "\n");
	}

	public void println(int n, int radix) {
		print(Integer.toString(n, radix)  + "\n");
	}

	public void println(long n, int radix) {
		print(Long.toString(n, radix)  + "\n");
	}

	
}

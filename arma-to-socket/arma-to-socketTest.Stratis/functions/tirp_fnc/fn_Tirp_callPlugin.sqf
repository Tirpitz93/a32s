//thanks to killzone kid: http://killzonekid.com/

[format [ 
		"r:%1",
		"arma-to-socket" callExtension format [
			"s:%1", 
			_this select 0 
			]
		], 
		_this select 1]  spawn { 
			waitUntil {
				_res = "arma-to-socket" callExtension (_this select 0); 

				if (_res != "WAIT") exitWith { _res call (_this select 1); 
				true 
			}; 
			false 
		};
	}; 
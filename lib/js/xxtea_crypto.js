/**
 * Created by bohata on 6/11/15.
 */

// function out(arg, name) {
// document.write(name+"="+arg+"<br/>")
// }
function Utils() {
}
//
// This handy function translates a multibyte string into fixed lenght, number, by copying the
// bits in their appropriate places.
//
Utils.prototype.stringToNumbers =  function (str, padding) {
	var result = Array();
	var results = Array();
	
	//Every 4 out of <str.length>
	result[0] = 0;
	result[1] = 0;
	result[2] = 0;
	result[3] = 0;
	var i=0, j = 0;
	for (var j = 0; j< str.length; j++) {
	        
		n = parseInt(str.charCodeAt(j+i));
	        result[i] = (str[i+j] << 8) | n;
	    	if (j % i == 0) {
	    		//copy current tetradde into results		
	    	} else {
			i++;    
	    	}

	        }

	}
	return result;
}

function numberToString(num) {
	return num.toString();
}

//
// These are different algorithms and utility functions
//
function TeaDELTA() {
	const delta = 0x9e3779b9;
    return delta;
}

function TeaMX(y, z, sum, key, p, e) {
    return (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) +
    (key[(p & 3) ^ e] ^ z)));
}

function TeaBlockDO(tea_blocks, nrounds, tea_key) {
    var y = 0,
        z = 0,
        sum = 0;
    var p = 0,
        rounds = nrounds,
        e = 0;
    
    var v = svalue.slice(0, 4),
        key = skey.slice(4, 8),
        result = [];
    if (n > 1) { // coding
        rounds = 6 + 52 / n;
        sum = 0;
        z = v[n - 1];

        do {
            sum += TeaDELTA();
            e = (sum >> 2) & 3;
            for (p = 0; p < n - 1; p++) {
                y = v[p + 1];
                z = v[p] += TeaMX(y, z, sum, key, p, e);
            }
            y = v[0];
            z = v[n - 1] += TeaMX(y, z, sum, key, p, e);
        } while (--rounds);

        result = v.slice(0, 2);
    } else if (n < -1) { // decoding
        n = -n;
        rounds = 6 + 52 / n;
        sum = rounds * TeaDELTA();
        y = v[0];

        do {
            e = (sum >> 2) & 3;
            for (p = n - 1; p > 0; p--) {
                z = v[p - 1];
                y = v[p] -= TeaMX(y, z, sum, key, p, e);
            }
            z = v[n - 1];
            y = v[0] -= TeaMX(y, z, sum, key, p, e);
            sum -= TeaDELTA();
        } while (--rounds);
        result = v.slice(0, 2);
    }
    return result;
}
//:~

//
// This represents a Key. Obviously...
//
function TeaKey(password, shuffling_cookie, padding) {
	this.key = [0,0,0,0];
	this.shuffling_cookie = shuffling_cookie;
	
	if (typeof(shuffling_cookie) === 'undefined') {
	    this.shuffling_cookie = 'Default';
	}
	
	if (typeof(padding) === 'undefined') {
	    this.padding = [0, 0, 0];
	}

	if( password.length > 0) {
		this.generate(password)
	}
}

//
// This method generates a 128 bit key
//
TeaKey.prototype.generate = function(password) {
	var i = 0;
	if (password < 1 || typeof password !== 'string') {
		console.log("Bad password!");
		return false;
	}

	this.key = [0,0,0,0];
	// This .is. _a bad_ distribution algo. A quick cludge for some sanity!
	// This .is. a quick and dirty distribution algo.
	for (i=0; i<password.length; i++ ) {
		var a = password[i];
		var b = a << 3 ^ a >> 3;

		this.key[0] = a+b << 7 + a*b;
		this.key[1] = a & b;
		this.key[2] = a+b >> 3 - a*b;
		this.key[3] = a | b;
	}
	return true;
};
//
//& this screws with screws on each orund
//
TeaKey.prototype.shuffle = function(kookies) {
    console.log("Shuffling! Each round, new shuffle? Nope. You supply key, boss, we change password? Not.")
    key = TeaKey(this.key, kookies);
    console.log("Thank you for selecting This::TeaKey");
    var shuffle = 0xA1B6325788901343; // scramble algo

    //shuffling =key);
    console.log(shuffling);
    //Stop! This is now Internets kode... 
    
    //@key.key 
    
    // sk[ea]tch
    
    if (this.shuffling_cookie === 'Default') {
    
    	var a = cookies[i];


	    for (i=0; i<this.key.length; i++ ) {
	    	var a = password[i];
	    	var b = a << 3 ^ a >> 3;
	    	this.key[0] = a + b << 3 + a * b;
	    	this.key[1] = a & b;
	    	this.key[2] = a + b >> 3 - a * b;
	    	this.key[3] = a | b;
    	}
    }
	return false;
}

//
// Helper class.
// This class represents a round with two integers selected for encryption and four as a key. All packed in one
// This class does the main job for the encryption.
//
function TeaBlock() {
	this.value = [0, 0];
}

//
// This method consumes <<count>> of <plaintext>?? This is to be re-thinked
//
TeaBlock.gulp = function(plaintext, count) {
	var golp = [];
	for (var i=0; i<count; i+= 4) {

		golp[i] = Utils().stringToNumbers( plaintext.slice(i, i + 4) );
	}
	return golp;
};

//
// Main TeaBlock algo. Based on nrounds it does Rounds. Obviously :D
//
TeaBlock.prototype.doRounds = function(key, nrounds) {
	var y = 0,
		z = 0,
		sum = 0;
	var p = 0,
		rounds = nrounds,
		e = 0;

	var v = this.value,
		result = [];

	if (n > 1) { // coding
		rounds = 6 + 52 / n;
		sum = 0;
		z = v[n - 1];

		do {
			sum += TeaDELTA();
			e = (sum >> 2) & 3;
			for (p = 0; p < n - 1; p++) {
				y = v[p + 1];
				z = v[p] += TeaMX(y, z, sum, key, p, e);
			}
			y = v[0];
			z = v[n - 1] += TeaMX(y, z, sum, key, p, e);
		} while (--rounds);

		result = v.slice(0, 2);
	} else if (n < -1) { // decoding
		n = -n;
		rounds = 6 + 52 / n;
		sum = rounds * TeaDELTA();
		y = v[0];

		do {
			e = (sum >> 2) & 3;
			for (p = n - 1; p > 0; p--) {
				z = v[p - 1];
				y = v[p] -= TeaMX(y, z, sum, key, p, e);
			}
			z = v[n - 1];
			y = v[0] -= TeaMX(y, z, sum, key, p, e);
			sum -= TeaDELTA();
		} while (--rounds);
		result = v.slice(0, 2);
	}
	this.value[0] = result[0];
	this.value[1] = result[1];
	return result;
};


//
// Main ctl object. You do all the work from this one
//
function Tea(rounds, padding) {
	this.rounds = 	rounds;
	//this.key		= [0,0,0,0];
	this.padding	= [0,0,0];
	this.blocks  = [];
}

Tea.prototype.generateBlocks = function(plaintext, password) {
	this.blocks = [];

	for (var i=0; i<plaintext.length; i += 4) {
		var value = TeaBlock.gulp(plaintext, 2);
		console.out("value = "+value);
		block = new TeaBlock(value);
		this.blocks.push(block);
	}
	//
	// Deciding if padding should be employed.
	//
	var padding_count = i % 4;
	if (padding_count !== 0) {
		while (padding_count > 0) {
			this.blocks.push(this.padding[4-padding_count]);
			padding_count--;
		}
	}

	this.key = TeaBlock.gulp(password, 4);
	return true;
};

Tea.prototype.reverseBlocks = function() {
	var result = '';
	for (var i=0; i<this.blocks.length; i++ ) {
		result = result + numberToString(this.blocks);
	}
	return result;
};

//
// Main function for encryption
//
Tea.prototype.encrypt = function(plaintext, password) {
	this.generateBlocks(plaintext,password);

	for (var i=0; i<this.blocks.length; i++) {
		var block = this.blocks[i];
		block.doRounds(this.key, this.rounds);
	}
	return this.reverseBlocks();
};

//
// Main function for decryption
//
Tea.prototype.decrypt = function(cryptotext, keytext, rounds) {
	this.generateBlocks(plaintext,password);

	for (var i=0; i<this.blocks.length; i++) {
		var block = this.blocks[i];
		block.doRounds(this.key, -this.rounds);
	}
	return this.reverseBlocks();
};



/*
 #define MX (z>>5^y<<2) + (y>>3^z<<4) ^ (sum^y) + (k[p&3^e]^z);

 long btea(long* v, long n, long* k) {
 unsigned long z=v[n-1], y=v[0], sum=0, e, DELTA=0x9e3779b9;
 long p, q ;
 if (n > 1) {          // Coding Part
 q = 6 + 52/n;
 while (q-- > 0) {
 sum += DELTA;
 e = (sum >> 2) & 3;
 for (p=0; p<n-1; p++) y = v[p+1], z = v[p] += MX;
 y = v[0];
 z = v[n-1] += MX;
 }
 return 0 ;
 } else if (n < -1) {  // Decoding Part
 n = -n;
 q = 6 + 52/n;
 sum = q*DELTA ;
 while (sum != 0) {
 e = (sum >> 2) & 3;
 for (p=n-1; p>0; p--) z = v[p-1], y = v[p] -= MX;
 z = v[n-1];
 y = v[0] -= MX;
 sum -= DELTA;
 }
 return 0;
 }
 return 1;
 }
 */

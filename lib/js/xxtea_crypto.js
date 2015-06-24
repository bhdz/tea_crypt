/**
 * Created by bohata on 6/11/15.
 */


window.xxtea_crypto = (function () {
    "use strict";

    var util = {
        //Re[ply]: 
        //http://stackoverflow.com/questions/3319974/do-bitwise-operators-other-than-shifts-make-any-mathematical-sense-in-base-10
        BitwiseAnd: function (value1, value2) {
            // this slows the bad guys
            // b1100 & b0011 (mod 2) ===
            //  ???? Fyck it, fyck the editOr
            return value1 & value2;
        },
        
        BitwiseOr: function (value1, value2) {
            // this slows the bad guys
            return value1 | value2;
        },
        
        BitwiseNegate: function (value) {
            // this slows the bad guys
            return ~value;
        },
        
        ShiftLeft: function (value, amount) {
            var s = value, t = 0;
            while (t < amount) {
                s *= 2;
                t += 1;
            }
            return s;
        },
        
        ShiftRight: function (value, amount) {
            var s = value, t = 0;
            t = 0;
            while (t < amount) {
                s /= 2;
                t += 1;
            }
            return s;
        },
        
        // block_size ** in bytes
        CreateBlock: function (block_size) {
            var result = [], i = 0;
            for (i = 0; i < block_size / 4; i += 1) {
                result.push(0);
                
            }
            
            return result;
        },
        
        // cleartext ** what is to be encrypted
        // padding   ** what is to be appended when string.length % 4
        // block_size ** in bytes
        StringToBlocks: function (cleartext, padding, block_size) {
            var result = [], block = [], gulp = "", ret = {}, code_point = '', nums = [0, 0, 0, 0], j = 0, k = 0, i = 0, b = 0, p = 0, q = 0, m = 0, n = 0, r = 0, s = 0, t = 0;
            
            block = util.CreateBlock(block_size);
            gulp = "";
            
            // Each byte in the string belongs to a block
            for (i = 0; i < cleartext.length; i += 1) {
                code_point = cleartext.charAt(i); 
                gulp += code_point; // LOL@js: gulp'ing <block_size> .of: <code_points> and building strings

                if (j < block_size - 1) {
                    j += 1;
                } else {
                    for (b = 0; b < block_size; b += 4) {
                        // URGENT FIXAGE.6:
                        p = parseInt(gulp.charCodeAt(b * block_size    ), 10); //Leftmost byte
                        q = parseInt(gulp.charCodeAt(b * block_size + 1), 10);
                        m = parseInt(gulp.charCodeAt(b * block_size + 2), 10);
                        n = parseInt(gulp.charCodeAt(b * block_size + 3), 10); //Rigthmost byte
                        
                        alert("StringToBlocks:p="+p);
                        alert("StringToBlocks:q="+q);
                        alert("StringToBlocks:m="+m);
                        alert("StringToBlocks:n="+n);
                        
                        r = p << 24 | q << 16 | m << 8 | n;
                        
                        block.push(r);
                    }
 
                    result.push(block);
                    
                    block = util.CreateBlock(block_size);
                    
                    j = 0;
                    gulp = "";
                    p = 0;
                    q = 0;
                    m = 0;
                    n = 0;
                    r = 0;
                }
            }

            //i = 0;
            //
            //padding
            //block = util.CreateBlock(j);
            // fill in the rest of it with padding
            //while (j > 0) {
            //    code_point = padding.charAt(i), 10);
            // ...
            //}
            //result.push(block);
            
            //yes baby, one giant gulp of coding mess... 
            // this has to be fixed 
            //
            // **todo:** Disect these whole sections into smaller co-working 
            // partners 
            
            //
            //testing ** not yet, this is in it's inception phase
            ret = {
                blocks_remaining: j,
                blocks: result
            };
            return ret;
        },
        
        // cleartext ** what is to be encrypted
        // padding   ** what is to be appended when string.length % 4
        // block_size ** in bytes
        StringToBlocks2: function (cleartext, padding, block_size) {
            var result = [], block = [], gulp = "", ret = {}, code_point = '', nums = [0, 0, 0, 0], j = 0, k = 0, i = 0, b = 0, p = 0, q = 0, m = 0, n = 0, r = 0, s = 0, t = 0;
            //add padding here:: based on <cleartext.length % block_size >
            
            // no gulp-ing chopping code here:: This is {even} better than the original::
            for (i = 0; i < cleartext.length; i += 1) {
                if (i % block_size == 0) {
                    r = 0;
                    for (j = 0; j < block_size; j += 1) {
                        code_point = cleartext.charAt(i + b * block_size + j); 
                        p = parseInt(code_point, 10);
                        r = r | p << (8 * block_size );

                    }
                    block.push(r);
                    b += 1;
                    result.push(block);
                }
            }
            //release candidate:: <StringToBlocks = StringToBlocks2>:: Reader(), Sit back and relax
           
            ret = {
                blocks_remaining: j,
                blocks: result
            };
            return ret;
        },
        
        BlocksToString: function (blocks, block_size) {
            var result = {
                padding: "",
                string: ""
            }, j = 0, i = 0, k = 0, numbers = [], code_point = 'a';
            
            result.padding = "";
            result.string = "";
            
            for (i = 0; i < blocks.length; i += 1) {
                numbers[j] = blocks[i];
                
                if (j < block_size) {
                    j += 1;
                } else {
                    for (k = 0; k < block_size; k += 1) {
                        code_point = String.fromCharCode(numbers[j]);
                        
                    }
                    j = 0;
                }

                result.string += code_point;
            }
            return result;
        }
    };
    
    //
    // These are different algorithms and utility functions
    //
    function TeaDELTA() {
        var delta = 0x9e3779b9;
        return delta;
    }

    function TeaMX(y, z, sum, key, p, e) {
        //return (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) +
        //(key[(p & 3) ^ e] ^ z)));
        var ret = {
            unfinished: true
        };
        return ret;
    }

    function TeaBlockDO(tea_blocks, nrounds, tea_key) {
        /*var y = 0,
            z = 0,
            sum = 0,
            p = 0,
            rounds = nrounds,
            e = 0;
    
            v = svalue.slice(0, 4),
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
    }*/
        var result = {
            unfinished: true
        };
        return result;
    }
    
    //:~

    //
    // This represents a Key. Obviously...
    //
    function TeaKey(password, shuffling_cookie, padding) {
        this.key = [0, 0, 0, 0];
        this.shuffling_cookie = shuffling_cookie;
	
        if (typeof (shuffling_cookie) === 'undefined') {
            this.shuffling_cookie = 'Default';
        }
	
        if (typeof (padding) === 'undefined') {
            this.padding = [0, 0, 0];
        }

        if (password.length > 0) {
            this.generate(password);
        }
        return this;
    }

    //
    // This method generates a 128 bit key
    //
    TeaKey.prototype.generate = function (password) {
        var i = 0, a = null, b = null;
        if (password < 1 || typeof password !== 'string') {
            throw ("Bad password!");
            
        }
		
        this.key = [0, 0, 0, 0];
        // This .is. _a bad_ distribution algo. A quick cludge for some sanity!
        // This .is. a quick and dirty distribution algo.
        for (i = 0; i < password.length; i += 1) {
            a = password[i];
            
            /*b = a << 3 ^ a >> 3;
            this.key[0] = a+b << 7 + a*b;
            this.key[1] = a & b;
            this.key[2] = a+b >> 3 - a*b;
            this.key[3] = a | b;
            */
        }
        return true;
    };
    
    //
    //& this screws with screws on each orund
    //
    TeaKey.prototype.shuffle = function (kookies) {
    /*    
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
        }*/
        return false;
    };

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
    TeaBlock.gulp = function (plaintext, count) {
        /*var golp = [], i;
        for (i = 0; i < count; i += 4) {
            //golp[i] = utils.stringToNumbers( plaintext.slice(i, i + 4) );
        }
        return golp;*/
        
        var result = {
            unfinished: true
        };
        return result;
    };

    //
    // Main TeaBlock algo. Based on nrounds it does Rounds. Obviously :D
    //
    TeaBlock.prototype.doRounds = function (key, nrounds) {/*
        var y = 0,
            z = 0,
            sum = 0,
            p = 0,
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
	return result;*/
        var result = {
            unfinished: true
        };
        return result;
    };


    //
    // Main ctl object. You do all the work from this one
    //
    function Tea(rounds, padding) {
        /*
	   this.rounds = rounds;
	   //this.key		= [0,0,0,0];
	   this.padding	= [0,0,0];
	   this.blocks  = [];*/
        var result = {
            unfinished: true
        };
        return result;
    }

    Tea.prototype.generateBlocks = function (plaintext, password) {
        /*
        this.blocks = [];

        for (var i=0; i<plaintext.length; i += 4) {
            var value = TeaBlock.gulp(plaintext, 2);

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
    
	return true;*/
        var result = {
            unfinished: true
        };
        return result;
    };

    Tea.prototype.reverseBlocks = function () {
        /*
	   var result = '';
	   for (var i=0; i<this.blocks.length; i++ ) {
           result = result + numberToString(this.blocks);
	}
	return result;*/
        var result = {
            unfinished: true
        };
        return result;
    };

    //
    // Main function for encryption
    //
    Tea.prototype.encrypt = function (plaintext, password) {
        /*
	this.generateBlocks(plaintext,password);

	for (var i=0; i<this.blocks.length; i++) {
		var block = this.blocks[i];
		block.doRounds(this.key, this.rounds);
	}
	return this.reverseBlocks();*/
        var result = {
            unfinished: true
        };
        return result;
    };

    //
    // Main function for decryption
    //
    Tea.prototype.decrypt = function (cryptotext, keytext, rounds) {
        /*
	   this.generateBlocks(plaintext,password);

	   for (var i=0; i<this.blocks.length; i++) {
		var block = this.blocks[i];
		block.doRounds(this.key, -this.rounds);
	   }
	   return this.reverseBlocks();
       */
        var result = {
            unfinished: true
        };
        return result;
    };
    
    function Tea_Block(blocks) {
        
    }
    
    return {
        util: util,
        Tea: Tea,
        TeaBlock: TeaBlock,
        TeaKey: TeaKey
    };

}());

/*
 * lex_rotors.h	should be the same as rotlist.h		--Dale
 *		Need to build a script to autogenerate this!
 */

#ifndef _LEX_ROTORS_H
#define _LEX_ROTORS_H

#define LEXROT(r)	{ #r , ROT_MODEL_ ## r } ,

struct lexriglist_s lex_rotlist[] = {
	LEXROT( NONE )
	LEXROT( DUMMY )
	LEXROT( RPC )
	LEXROT( EASYCOMM1 )
	LEXROT( EASYCOMM2 )
	LEXROT( FODTRACK )
	LEXROT( ROTOREZ )
	LEXROT( ROTORCARD )
	LEXROT( DCU )
//	LEXROT(  )
	{ NULL, 0 }
};

#endif	// _LEX_ROTORS_H
// end lex_rotors.h

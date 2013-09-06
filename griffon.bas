'
' If you're using fbc 0.17 or later, compile with:
' fbc griffon.bas -x build/griffon.exe -lang fblite
'
' in game scripts
'	2 - find master key
'	3 - find crystal
'	4 - find shield - obj 8
'	5 - find sword - obj 9
'   6 - regular key chest
'   7 - blue flask
'   8 - garden's master key
'   9 - lightning bomb
'  10 - blue flask chest
'  11 - lightning chest
'  12 - armour chest
'  13 - citadel master key
'  14 - end of game
'  15 - get sword3
'  16 - shield3
'  17 - armour3
'   20 key chest 1
'   60-lever
'
'


' monsters
' 1 - baby dragon
' 2 - one wing
' 3 - boss 1
' 4 - black knight
' 5 - fire hydra
' 6 - red dragon
' 7 - priest
' 8 - yellow fire dragon
' 9 - two wing
'10 - dragon2
'11 - final boss
'12 - bat kitty

' chests
'  0 - regular flask
' 11 - key chest
' 14 - blue flask chest
' 15 - lightning chest
' 16 - armour chest
' 17 - citadel master key
' 18 - sword3
' 19 - shield3
' 20 - armour3
DEFINT A-Z

'$include: 'windows.bi'
'$include: 'SDL\SDL.bi'
'$include: 'SDL\SDL_image.bi'
'$include: 'win\mmsystem.bi'
'$INCLUDE: 'fmod.bi'

Randomize Timer

CONST maxnpc = 32, maxfloat = 32, maxspell = 32
const ice = 0, steel = 1, wood = 2, rock = 3, fire = 4
'inventory items
const flask = 0, doubleflask = 1, shock = 2, normalkey = 3, masterkey = 4

const sndbite = 0, sndcrystal = 1, snddoor = 2, sndenemyhit = 3, sndice = 4, sndlever = 5, sndlightning = 6
const sndmetalhit = 7, sndpowerup = 8, sndrocks = 9, sndswordhit = 10, sndthrow = 11, sndchest = 12, sndfire = 13, sndbeep = 14

type playertype
	px	as single
	py	as single
	opx	as single
	opy	as single
	walkdir		as integer
	walkframe	as single
	walkspd		as single
	attackframe	as single
	attackspd	as single

	hp	as integer
	maxhp	as integer
	hpflash	as single
	hpflashb	as integer
	level	as integer
	sword	as integer
	shield	as integer
	armour	as integer
	foundspell(5)	as integer
	spellcharge(5)	as single
	flasks	as integer
	foundcrystal	as integer
	crystalcharge	as single
	attackstrength	as single
	spellstrength   as single
	spelldamage	as integer
	sworddamage	as integer

	masterkey	as integer

	exp	as integer
	nextlevel	as integer

	windowloc	as integer
	pause	as integer

	itemselshade	as single
	ysort   as integer
end type

type bodysectiontype

	x	as single
	y	as single
	parentID	as integer
	isbase	as integer
	sprite	as integer
	bonelength	as integer  'the 'bone' that connects the body sections
end type


type npctype
	x	as single
	y	as single
	spriteset	as integer
	x1	as integer  'patrol area
	y1	as integer
	x2	as integer
	y2	as integer
	attitude	as integer
	hp	as integer

	maxhp	as integer
	item1	as integer
	item2	as integer
	item3	as integer
	script	as integer
	frame	as single
	frame2  as single 'end boss specific
	cframe	as integer
	onmap	as integer 'is this npc set to be genned in the mapfile

	ticks	as integer
	pause	as integer
	shake	as integer

	movementmode	as integer
	walkdir	as integer
	walkspd	as single
	movingdir	as integer
	moving	as integer

	attacking	as integer
	attackframe	as single
	cattackframe	as integer
	attackspd	as single
	attackdelay	as integer
	attacknext	as integer
	attackattempt	as integer

	spelldamage	as integer
	attackdamage	as integer


	'onewing and firehydra specific
	bodysection(30) as bodysectiontype
	swayangle	as single
	swayspd	as single
	headtargetx(3)	as single
	headtargety(3)	as single
	castpause	as integer

	'firehydra specific
	attacknext2(3)	as integer
	attacking2(3)	as integer
	attackframe2(3)	as integer

	'dragon2 specific
	floating	as single
end type


type spelltype
	spellnum	as integer
	homex	as single
	homey	as single
	enemyx	as single
	enemyy	as single

	frame	as single

	damagewho	as integer '0 = npc, 1 = player

	'for earthslide
	rocky(8)	as single
	rockimg(8)	as integer
	rockdeflect(8)	as integer

	strength	as single

	'fire
	legalive(4)	as integer

	'spell 6 specific
	fireballs(6,3)	as single
		'x,y,targetx, targety
	nfballs	as integer
	ballon(6)	as integer

	npc	as integer
end type



type animset2type
	x	as integer 'xyloc on spriteimageset
	y	as integer
	xofs	as integer 'the actual place to paste the sprite in reference to the bodypart loc on screen
	yofs	as integer
	w	as integer 'w/h of the sprite in the imageset
	h	as integer
end type


declare sub game_addFloatIcon (ico, xloc!, yloc!)
declare sub game_addFloatText (stri$, xloc!, yloc!, col%)
declare sub game_attack ()
declare sub game_castspell (spellnum%, homex!, homey!, enemyx!, enemyy!, damagewho%)
declare sub game_checkhit ()
declare sub game_checkinputs ()
declare sub game_configmenu()
declare sub game_damagenpc (npcnum%, damage%, spell%)
declare sub game_damageplayer (damage%)
declare sub game_drawanims (Layer%)
declare sub game_drawhud ()
declare sub game_drawnpcs (mode%)
declare sub game_drawover (modx%, mody%)
declare sub game_drawplayer ()
declare sub game_drawview ()
declare sub game_endofgame ()
declare sub game_eventtext (stri$)
declare sub game_handlewalking ()
declare sub game_loadmap (mapnum%)
declare sub game_main ()
declare sub game_newgame ()
declare sub game_playgame ()
declare sub game_processtrigger (trignum%)
declare sub game_saveloadnew ()
declare sub game_showlogos ()
declare sub game_swash ()
declare sub game_theend ()
declare sub game_title (mode%)
declare sub game_updanims ()
declare sub game_updatey()
declare sub game_updmusic ()
declare sub game_updnpcs ()
declare sub game_updspells ()
declare sub game_updspellsunder ()

declare sub sys_customLoad ()
declare sub sys_Initialize ()
declare sub sys_line (byval buffer as SDL_Surface ptr, x1%, y1%, x2%, y2%, col%)
declare sub sys_LoadAnims ()
declare sub sys_LoadFont ()
declare sub sys_LoadItemImgs ()
declare sub sys_LoadTiles ()
declare sub sys_loadTriggers()
declare sub sys_print (byval buffer as SDL_Surface ptr, stri$, xloc%, yloc%, col%)
declare sub sys_progress (w%, wm%)
declare sub sys_LoadObjectDB ()
declare sub sys_setupAudio ()
declare sub sys_update ()

'system
dim shared video as SDL_Surface ptr, videobuffer as SDL_Surface ptr, videobuffer2 as SDL_Surface ptr, videobuffer3 as SDL_Surface ptr
dim shared titleimg as SDL_Surface ptr, titleimg2 as SDL_Surface ptr, inventoryimg as SDL_Surface ptr
dim shared logosimg as SDL_Surface ptr, theendimg as SDL_Surface ptr
dim shared event as SDL_Event
dim shared SCR_WIDTH, SCR_HEIGHT, SCR_BITS, SCR_TOPX, SCR_TOPY
dim shared ticks, tickspassed, nextticks
dim shared fp, fps, fpsr as single
dim shared mapbg as SDL_Surface ptr, clipbg as SDL_Surface ptr, clipsurround(3,3) as uinteger, clipbg2 as SDL_Surface ptr
dim shared fullscreen
dim shared walklimits(6) = {5 * 16, 5 * 16, 14 * 16, 9 * 16, 320, 144}
dim shared keys as Uint8 ptr
dim shared animspd as single, rampdata(40,24)
dim shared joystickinfo as JOYINFO
dim shared curmap, fontchr(224, 4) as SDL_Surface ptr
dim shared itemimg(20) as SDL_Surface ptr, windowimg as SDL_Surface ptr
dim shared spellimg as SDL_Surface ptr

dim shared itemselon, curitem, itemticks, itemyloc as single
dim shared selenemyon, curenemy, forcepause
dim shared roomlock ' set to disable any room jumps while inthe room
dim shared scriptflag (99, 9), saveslot
		'script, flag

dim shared secsingame, secstart
dim shared story$(37), mapimg(3) as SDL_Surface ptr, invmap(3,13,7), story2$(37)

'options
dim shared opfullscreen, opmusic, opeffects, opmusicvol, opeffectsvol

dim shared rc2 as SDL_Rect, rc as SDL_Rect, rcSrc as SDL_Rect, rcDest as SDL_Rect, rcSrc2 as SDL_Rect


dim shared HWACCEL, HWSURFACE, maxlevel

'inventory
dim shared inventoryalpha, inventory(5)

'-----------special case
dim shared dontdrawover
		' used in map24 so that the candles dont draw over the boss, default set to 0

'saveload info
dim shared saveloadimg as SDL_Surface ptr


'post info
dim shared postinfo(20,2) as single, nposts

'cloud info
dim shared cloudimg as SDL_Surface ptr, clouddeg as single, cloudson

'spell info
dim shared spellinfo(maxspell) as spelltype


'player info
dim shared movingup, movingdown, movingleft, movingright
dim shared player as playertype, attacking, playera as playertype


'tile info
dim shared tiles(4) as SDL_Surface ptr
dim shared tileinfo(3, 40, 24, 2)
		'maplayer, x, y, tiledata (tile, tilelayer)

dim shared elementmap(20,15)


'animation info
dim shared anims(99) as SDL_Surface ptr
	   'id number 0&1 = players
dim shared animsa(99) as SDL_Surface ptr
	   'attack anims
dim shared playerattackofs(4, 16, 3) as single
	   '[dir] [frame] [x,y ofs, completed(0/1)]

dim shared floattext(maxfloat, 3) as single, floatstri$(maxfloat)
	   '[id] [framesleft, x, y, col]

dim shared floaticon(maxfloat, 3) as single
	   '[id] [framesleft, x, y, ico]

'special for animset2
dim shared animset2(6) as animset2type, animset9(6) as animset2type

'object info
dim shared objectframe(255, 1) AS SINGLE, lastobj
		   'frame!, curframe
dim shared objectinfo(32, 6)
		   'nframes,xtiles,ytiles,speed,type,script, update?
dim shared objecttile(32, 8, 2, 2, 1)
		   '[objnum] [frame] [x] [y] [tile/layer]
dim shared objmap(20, 14)

dim shared objmapf(999, 20, 14)
	   '[mapnum] x, y  set to 1 to make this objmap spot stay at -1

'trigger info
dim shared triggers(9999,9)
		'[map#][index], [var]
				'map#,x,y
dim shared triggerloc(320,240), ntriggers

'npc info
dim shared npcinfo(maxnpc) as npctype, lastnpc

'music info
dim shared mgardens as integer, mgardens2 as integer, mgardens3 as integer, mgardens4 as integer, mboss as integer
dim shared menabled as integer, musicchannel, mendofgame as integer, menuchannel, mmenu as integer
dim shared pgardens as integer, pboss as integer
dim shared loopseta
		loopseta = 0
dim shared sfx(20) as integer

'room locks
dim shared roomlocks(200), saidlocked, canusekey, locktype, roomtounlock, saidjammed
'set to 1 for normal key, set to 2 for master, set to 0 if unlocked

'dialog
dim shared dialogflags(999)

'ysort
dim shared ysort(2400), lasty, firsty


sys_initialize
game_showlogos
game_main


'element tile locations

data  2, 2, 2, 2,-1,-1,-1, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1
data  2,-1,-1,-1,-1,-1,-1, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1
data  2,-1, 2, 2,-1,-1,-1, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1
data  2,-1, 2,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data  2, 2, 2, 2, 2,-1,-1,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1, 2,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1, 0, 0, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
data -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1


data "Ever since I was a child"
data "I remember being told the"
data "Legend of the Griffon Knights,"
data "who rid the world of the"
data "Dragon Empire.  These great"
data "heroes inspired us to become"
data "knights as well."
data " "
data "Now, 500 years after the war"
data "ended, the Dragons have"
data "returned.  Cities are falling"
data "from the lack of knights to"
data "protect them."
data " "
data "We never saw it coming."
data " "
data "And now, here I am, making"
data "my way into the lower town"
data "of Fidelis, a small city on"
data "the main continent. The rest"
data "of my men have died over"
data "the last couple days from"
data "aerial attacks."
data " "
data "We believed we could find"
data "shelter here, only to find"
data "every last griffon dead,"
data "the town burned to the ground,"
data "and transformed into a garrison"
data "for the Dragon forces."
data " "
data "In these dark times, I try to"
data "draw strength from the stories"
data "of those knights that risked"
data "everything to protect their homeland,"
data " "
data "and hope that I can die"
data "with that honor as well."


data "After the fall of Margrave Gradius,"
data "All the dragons, struck with panic,"
data "evacuated the city immediately."
data " "
data "It's funny how without a leader"
data "everyone is so weak."
data " "
data " "
data "But yet another leader will rise,"
data "and another city will fall."
data " "
data " "
data "I should return home to Asherton"
data "It's time to leave this place"
data "and cleanse this blood stained"
data "life of mine."
data " "
data "No one should have to see as much"
data "death as I have."
data " "
data " "
data "Before, I said that I wanted"
data "to die an honorable death."
data " "
data "Now I say that I have lived an"
data "honorable life,"
data "and I am free to die as I please."




'map in inventory menu
'map 0
data 0,0,0,0,0,0,0,0,0,0,0,0,0
data 0,0,0,0,0,0,0,0,0,0,0,0,0
data 0,0,0,0,0,43,44,45,46,0,0,0,0
data 0,0,0,0,0,42,0,0,0,0,0,0,0
data 0,0,0,0,3,2,0,0,0,0,0,0,0
data 0,0,0,0,4,5,0,0,0,0,0,0,0
data 0,0,0,0,0,0,0,0,0,0,0,0,0

'map 1
data 0,0,0,0,0,0,0,0,0,0,0,0,0
data 0,0,0,24,0,0,0,0,0,0,0,0,0
data 0,0,19,20,21,22,0,0,0,27,0,0,0
data 0,0,16,17,18,0,0,0,29,30,31,0,0
data 0,0,12,0,13,14,0,32,33,34,35,36,0
data 0,8,7,6,9,10,0,37,38,39,40,41,0
data 0,0,0,0,0,0,0,0,0,0,0,0,0

'map 2
data 0,0,0,0,0,0,67,0,0,0,0,0,0
data 0,0,0,0,0,0,66,0,0,0,0,0,0
data 0,0,0,0,0,63,64,65,0,0,0,0,0
data 0,0,0,0,58,59,60,61,62,0,0,0,0
data 0,0,0,0,0,55,56,57,0,0,0,0,0
data 0,0,0,0,50,51,52,53,54,0,0,0,0
data 0,0,0,0,0,48,47,49,0,0,0,0,0

'map 3
data 0,0,0,0,0,0,0,0,0,0,0,0,0
data 0,0,0,82,0,0,0,0,0,0,0,0,0
data 0,0,0,79,80,81,0,74,72,0,0,0,0
data 0,0,0,78,0,0,0,73,70,69,68,0,0
data 0,0,77,76,75,0,0,0,71,0,0,0,0
data 0,0,0,0,0,0,0,0,0,0,0,0,0
data 0,0,0,0,0,0,0,0,0,0,0,0,0




sub game_addFloatIcon (ico, xloc!, yloc!)

	i = 0
	do
		if floaticon(i, 0) = 0 then
			floaticon(i, 0) = 32
			floaticon(i, 1) = xloc!
			floaticon(i, 2) = yloc!
			floaticon(i, 3) = ico

			exit sub
		end if
		i = i + 1
		if i = maxfloat+1 then exit do
	loop

end sub


sub game_addFloatText (stri$, xloc!, yloc!, col)

	i = 0
	do
		if floattext(i, 0) = 0 then
			floattext(i, 0) = 32
			floattext(i, 1) = xloc!
			floattext(i, 2) = yloc!
			floattext(i, 3) = col
			floatstri$(i) = stri$
			exit sub
		end if
		i = i + 1
		if i = maxfloat+1 then exit do
	loop

end sub


sub game_attack ()

	dim npx as single, npy as single

	npx = player.px + 12
	npy = player.py + 20

	lx = (npx - (npx MOD 16)) / 16
	ly = (npy - (npy MOD 16)) / 16

	if player.walkdir = 0 then
		if ly > 0 then
			o = objmap (lx, ly - 1)
			if ly > 1 and curmap = 58 then o2 = objmap(lx, ly - 2)
			if ly > 1 and curmap = 54 then o2 = objmap(lx, ly - 2)

			'cst
			if (objectinfo(o, 4) = 1 and (o = 0 or o > 4)) or (objectinfo(o2, 4) = 0 and o2 = 10) then
				if o2 = 10 then o = 10

				oscript = objectinfo(o, 5)
				if oscript = 0 and inventory(0) < 9 then
					inventory(0) = inventory(0) + 1
					if inventory(0) > 9 then inventory(0) = 9
					game_addFloatIcon 6, lx * 16, (ly - 1) * 16

					objmapf (curmap, lx, ly - 1) = 1

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found Flask!"
					itemticks = ticks + 215

					exit sub
				end if
				if oscript = 0 and inventory(0) = 9 then

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndchest))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					game_eventtext "Cannot Carry any more Flasks!"
					itemticks = ticks + 215

					exit sub
				end if
				if oscript = 2 then
					inventory(4) = inventory(4) + 1

					game_addFloatIcon 14, lx * 16, (ly - 1) * 16

					itemticks = ticks + 215

					if curmap = 34 then scriptflag(2,0) = 2
					if curmap = 62 then scriptflag(8,0) = 2
					if curmap = 81 then scriptflag(13,0) = 2

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Temple Key!"

					exit sub
				end if
				if oscript = 3 then
					player.foundcrystal = 1
					player.crystalcharge = 0

					game_addFloatIcon 7, lx * 16, (ly - 1) * 16

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Infinite Crystal!"
					itemticks = ticks + 215

					exit sub
				end if
				if oscript = 4 and player.shield = 1 then
					player.shield = 2

					game_addFloatIcon 4, lx * 16, (ly - 1) * 16

					itemticks = ticks + 215

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Obsidian Shield!"

					objmapf (4, 1, 2) = 1

					exit sub
				end if

				if oscript = 5 and player.sword = 1 then
					player.sword = 2

					game_addFloatIcon 3, lx * 16, (ly - 1) * 16

					itemticks = ticks + 215

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Fidelis Sword!"

					exit sub
				end if

				if oscript = 6 then
					if inventory(3) < 9 then
						inventory(3) = inventory(3) + 1

						for s = 20 to 23
							if scriptflag(s, 0) = 1 then
								scriptflag(s, 0) = 2
							end if
						next

						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						objmapf (curmap, lx, ly - 1) = 1

						if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

						game_eventtext "Found Key"

						game_addFloatIcon 16, lx * 16, (ly - 1) * 16
					else

						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndchest))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						game_eventtext "Cannot Carry Any More Keys"
					end if
				end if

				if oscript = 7 and inventory(1) < 9 then
					inventory(1) = inventory(1) + 1
					if inventory(1) > 9 then inventory(1) = 9
					game_addFloatIcon 12, lx * 16, (ly - 1) * 16

					objmapf (curmap, lx, ly - 1) = 1

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found Mega Flask!"
					itemticks = ticks + 215

					exit sub
				end if
				if oscript = 7 and inventory(1) = 9 then

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndchest))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					game_eventtext "Cannot Carry any more Mega Flasks!"
					itemticks = ticks + 215

					exit sub
				end if

				if oscript = 10 and inventory(1) < 9 then
					inventory(1) = inventory(1) + 1
					if inventory(1) > 9 then inventory(1) = 9
					game_addFloatIcon 12, lx * 16, (ly - 1) * 16

					objmapf (curmap, lx, ly - 1) = 1

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found Mega Flask!"
					itemticks = ticks + 215

					exit sub
				end if
				if oscript = 10 and inventory(1) = 9 then

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndchest))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					game_eventtext "Cannot Carry any more Mega Flasks!"
					itemticks = ticks + 215

					exit sub
				end if

				if oscript = 11 and inventory(2) < 9 then
					inventory(2) = inventory(2) + 1
					if inventory(2) > 9 then inventory(2) = 9
					game_addFloatIcon 17, lx * 16, (ly - 1) * 16

					objmapf (curmap, lx, ly - 1) = 1

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found Lightning Bomb!"
					itemticks = ticks + 215

					exit sub
				end if
				if oscript = 11 and inventory(2) = 9 then

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndchest))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					game_eventtext "Cannot Carry any more Lightning Bombs!"
					itemticks = ticks + 215

					exit sub
				end if

				if oscript = 12 and player.armour = 1 then
					player.armour = 2

					game_addFloatIcon 5, lx * 16, (ly - 1) * 16

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Fidelis Mail!"
					itemticks = ticks + 215

					exit sub
				end if

				if oscript = 60 then
					if curmap = 58 and scriptflag(60,0) = 0 then
						scriptflag(60,0) = 1

						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndlever))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

					elseif curmap = 58 and scriptflag(60,0) > 0 then
						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(snddoor))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						game_eventtext "It's stuck!"
					end if
					if curmap = 54 and scriptflag(60,0) = 1 then
						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndlever))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						scriptflag(60,0) = 2
					elseif curmap = 54 and scriptflag(60,0) > 1 then
						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(snddoor))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						game_eventtext "It's stuck!"
					end if

				end if

				if oscript = 15 and player.sword < 3 then
					player.sword = 3

					game_addFloatIcon 18, lx * 16, (ly - 1) * 16

					itemticks = ticks + 215

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Blood Sword!"

					objmapf (4, 1, 2) = 1

					exit sub
				end if

				if oscript = 16 and player.shield < 3 then
					player.shield = 3

					game_addFloatIcon 19, lx * 16, (ly - 1) * 16

					itemticks = ticks + 215

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Entropy Shield!"

					objmapf (4, 1, 2) = 1

					exit sub
				end if

				if oscript = 17 and player.armour < 3 then
					player.armour = 3

					game_addFloatIcon 20, lx * 16, (ly - 1) * 16

					itemticks = ticks + 215

					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					if objectinfo(o, 4) = 1 then objmap (lx, ly - 1) = 3

					game_eventtext "Found the Rubyscale Armour!"

					objmapf (4, 1, 2) = 1

					exit sub
				end if

			end if
		end if
	end if

	attacking = 1

	player.attackframe = 0


	movingup = 0
	movingdown = 0
	movingleft = 0
	movingright = 0

	for i = 0 to 15
		for a = 0 to 3
			playerattackofs (a, i, 2) = 0
		next
	next

end sub


sub game_castspell (spellnum, homex!, homey!, enemyx!, enemyy!, damagewho)

	'spellnum 7 = sprite 6 spitfire
	i = 0
	do
		if spellinfo(i).frame = 0 then
			spellinfo(i).homex = homex!
			spellinfo(i).homey = homey!
			spellinfo(i).enemyx = enemyx!
			spellinfo(i).enemyy = enemyy!
			spellinfo(i).spellnum = spellnum
			dw = 0
			npc = 0
			if damagewho > 0 then
				dw = 1
				npc = damagewho
			end if

			spellinfo(i).damagewho = dw
			spellinfo(i).npc = npc

			spellinfo(i).frame = 32
			if damagewho = 0 then
				spellinfo(i).strength =  player.spellstrength / 100
				if player.spellstrength = 100 then spellinfo(i).strength = 1.5
			end if

			'set earthslide vars
			if spellnum = 2 then
				for f = 0 to 8

					spellinfo(i).rocky(f) = 0
					spellinfo(i).rockimg(f) = int(rnd * 4)
					spellinfo(i).rockdeflect(f) = (int(rnd * 128) - 64) * 1.5

				next
			end if

			'set fire vars
			if spellnum = 3 then
				for f = 0 to 4

					spellinfo(i).legalive(f) = 32

				next
			end if


			'room fireball vars
			if spellnum = 6 then
				nballs = 0
				for x = 0 to 19
					for y = 0 to 14

						if objmap(x,y) = 1 or objmap(x,y) = 2 and nballs < 5 and int(rnd * 4) = 0 then
							ax = x * 16
							ay = y * 16
							bx = player.px + 4
							by = player.py + 4
							d! = sqr((bx - ax) ^ 2 + (by - ay) ^2)

							tx! = (bx - ax) / d!
							ty! = (by - ay) / d!

							spellinfo(i).fireballs(nballs,0) = ax
							spellinfo(i).fireballs(nballs,1) = ay
							spellinfo(i).fireballs(nballs,2) = 0
							spellinfo(i).fireballs(nballs,3) = 0

							spellinfo(i).ballon(nballs) = 1
							nballs = nballs + 1
						end if
					next
				next
				spellinfo(i).nfballs = nballs
			end if

			if menabled = 1 and opeffects = 1 then
				if spellnum = 1 then
					snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndthrow))
					FSOUND_setVolume(snd, opeffectsvol)
				elseif spellnum = 5 then
					snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndcrystal))
					FSOUND_setVolume(snd, opeffectsvol)
				elseif spellnum = 8 or spellnum = 9 then
					snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndlightning))
					FSOUND_setVolume(snd, opeffectsvol)
				end if
			end if

			exit sub
		end if
		i = i + 1
		if i = maxspell+1 then exit do
	loop


end sub



sub game_checkhit ()

	dim temp as uinteger ptr, opx as single, opy as single, npx as single, npy as single
	dim bgc as uinteger

	if attacking = 1 then

		FOR i = 1 TO lastnpc

			IF npcinfo(i).hp > 0 and npcinfo(i).pause < ticks and int(rnd * 2) = 0 THEN

				npx = npcinfo(i).x
				npy = npcinfo(i).y

				opx = npx
				opy = npy

				xdif = player.px - npx
				ydif = player.py - npy

				ps = player.sword
				if ps > 1 then ps = ps * .75
				damage = player.sworddamage * (1 + rnd * 1) * player.attackstrength / 100 * ps

				if player.attackstrength = 100 then damage = damage * 1.5

				hit = 0
				if player.walkdir = 0 then
					if abs(xdif) <= 8 and ydif >= 0 and ydif < 8 then hit = 1
				elseif player.walkdir = 1 then
					if abs(xdif) <= 8 and ydif <= 0 and ydif > -8 then hit = 1
				elseif player.walkdir = 2 then
					if abs(ydif) <= 8 and xdif >= -8 and xdif < 8 then hit = 1
				elseif player.walkdir = 3 then
					if abs(ydif) <= 8 and xdif <= 8 and xdif > -8 then hit = 1
				end if


				if hit = 1 then
					if menabled = 1 and opeffects = 1 then
						snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndswordhit))
						FSOUND_setVolume(snd, opeffectsvol)
					end if

					game_damagenpc i, damage, 0
				end if


			end if

		next


	end if
end sub


sub game_checkinputs ()



	d = JoyGetPos (0, @joystickinfo)


	ntickdelay = 175

	SDL_PollEvent(@event)

	keys = SDL_GetKeyState(null)

	nposts = 0

	for i = 0 to 20
		postinfo(i, 0) = 0
		postinfo(i, 1) = 0
	next

	for x = 0 to 19
		for y = 0 to 14
			o = objmap(x,y)
			if objectinfo(o, 4) = 3 then
				postinfo(nposts, 0) = x * 16
				postinfo(nposts, 1) = y * 16
				nposts = nposts + 1
			end if
		next
	next





	if attacking = 1 or (forcepause = 1 and itemselon = 0) then exit sub

	if event.type = SDL_KEYDOWN then
		select case event.key.keysym.sym
			case SDLK_ESCAPE
				if itemticks < ticks then game_title 1

			case SDLK_RETURN
				if keys[SDLK_LALT] or keys[SDLK_RALT] then
					if fullscreen AND SDL_FULLSCREEN then
						fullscreen = 0 OR HWACCEL OR SDL_SWSURFACE
					else
						fullscreen = SDL_FULLSCREEN  OR HWACCEL OR HWSURFACE
					end if

					video = SDL_SetVideoMode( SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)
					SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
				end if

			case SDLK_SPACE

				if itemselon = 0 and itemticks < ticks then game_attack

				'd = SDL_SaveBMP(video, "shot.bmp")


				if itemselon = 1 and itemticks < ticks then

					if curitem = 0 and inventory(0) > 0 then

						itemticks = ticks + ntickdelay

						heal = 50
						maxh = player.maxhp - player.hp

						if heal > maxh then heal = maxh

						player.hp = player.hp + heal

						t$ = "+" + ltrim$(rtrim$(str$(heal)))

						game_addFloatText t$, player.px + 16 - 4 * len(t$), player.py + 16, 5

						inventory(0) = inventory(0) - 1

						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						itemselon = 0
						forcepause = 0

					end if

					if curitem = 1 and inventory(1) > 0 then

						itemticks = ticks + ntickdelay

						heal = 200
						maxh = player.maxhp - player.hp

						if heal > maxh then heal = maxh

						player.hp = player.hp + heal

						t$ = "+" + ltrim$(rtrim$(str$(heal)))

						game_addFloatText t$, player.px + 16 - 4 * len(t$), player.py + 16, 5

						inventory(1) = inventory(1) - 1

						if menabled = 1 and opeffects = 1 then
							snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
							FSOUND_setVolume(snd, opeffectsvol)
						end if

						itemselon = 0
						forcepause = 0

					end if


					if curitem = 2 and inventory(2) > 0 then

						game_castspell 8, player.px, player.py, npcinfo(curenemy).x, npcinfo(curenemy).y, 0

						forcepause = 1

						inventory(2) = inventory(2) - 1

						itemticks = ticks + ntickdelay
						selenemyon = 0
						itemselon = 0

					end if

					if curitem = 3 and inventory(3) > 0 and canusekey = 1 and locktype = 1 then

						roomlocks(roomtounlock) = 0
						game_eventtext "UnLocked!"

						inventory(3) = inventory(3) - 1

						itemticks = ticks + ntickdelay
						selenemyon = 0
						itemselon = 0

						exit sub
					end if

					if curitem = 4 and inventory(4) > 0 and canusekey = 1 and locktype = 2 then

						roomlocks(roomtounlock) = 0
						game_eventtext "UnLocked!"

						inventory(4) = inventory(4) - 1

						itemticks = ticks + ntickdelay
						selenemyon = 0
						itemselon = 0

						exit sub
					end if

					if curitem = 5 and player.crystalcharge = 100 then

						game_castspell 5, player.px, player.py, npcinfo(curenemy).x, npcinfo(curenemy).y, 0

						player.crystalcharge = 0

						forcepause = 1

						itemticks = ticks + ntickdelay
						selenemyon = 0
						itemselon = 0

					end if


					if curitem > 5 and selenemyon = 1 then

						if curenemy <= lastnpc then
							game_castspell curitem - 6, player.px, player.py, npcinfo(curenemy).x, npcinfo(curenemy).y, 0
						else
							pst = curenemy - lastnpc - 1
							game_castspell curitem - 6, player.px, player.py, postinfo(pst,0), postinfo(pst,1), 0
						end if

						player.spellcharge(curitem - 6) = 0

						player.spellstrength = 0

						itemticks = ticks + ntickdelay
						selenemyon = 0
						itemselon = 0
						forcepause = 0

					end if

					if curitem > 5 and selenemyon = 0 and itemselon = 1 then
						if player.spellcharge(curitem - 6) = 100 then
							itemticks = ticks + ntickdelay

							selenemyon = 1

							i = 0
							do

								if npcinfo(i).hp > 0 then
									curenemy = i
									exit do
								end if
								i = i + 1
								if i = lastnpc + 1 then
									selenemyon = 0
									exit do
								end if
							loop

							if nposts > 0 and selenemyon = 0 then
								selenemyon = 1
								curenemy = lastnpc + 1
							end if
						end if

					end if


				end if


			case SDLK_LCTRL

				if itemticks < ticks then
					selenemyon = 0
					if itemselon = 1 then
						itemselon = 0
						itemticks = ticks + 220
						forcepause = 0
					else
						itemselon = 1
						itemticks = ticks + 220
						forcepause = 1
						player.itemselshade = 0
					end if
				end if

			case SDLK_RCTRL

				if itemticks < ticks then
					selenemyon = 0
					if itemselon = 1 then
						itemselon = 0
						itemticks = ticks + 220
						forcepause = 0
					else
						itemselon = 1
						itemticks = ticks + 220
						forcepause = 1
						player.itemselshade = 0
					end if
				end if


		end select
	end if



	if itemselon = 0 then
		movingup = 0
		movingdown = 0
		movingleft = 0
		movingright = 0
		if keys[SDLK_UP] then movingup = 1
		if keys[SDLK_DOWN] then movingdown = 1
		if keys[SDLK_LEFT] then movingleft = 1
		if keys[SDLK_RIGHT] then movingright = 1
	else
		movingup = 0
		movingdown = 0
		movingleft = 0
		movingright = 0

		if selenemyon = 1 then

			if itemticks < ticks then
				if keys[SDLK_LEFT] then
					origin = curenemy
					do
						curenemy = curenemy - 1
						if curenemy < 1 then curenemy = lastnpc + nposts
						if curenemy = origin then exit do
						if curenemy <= lastnpc and npcinfo(curenemy).hp > 0 then exit do
						if curenemy > lastnpc then exit do
					loop

					itemticks = ticks + ntickdelay
				end if
				if keys[SDLK_RIGHT] then
					origin = curenemy
					do
						curenemy = curenemy + 1
						if curenemy > lastnpc + nposts then curenemy = 1
						if curenemy = origin then exit do
						if curenemy <= lastnpc and npcinfo(curenemy).hp > 0 then exit do
						if curenemy > lastnpc then exit do
					loop
					itemticks = ticks + ntickdelay
				end if


				if curenemy > lastnpc + nposts then curenemy = 1
				if curenemy < 1 then curenemy = lastnpc + nposts
			end if


		else

			if itemticks < ticks then
				if keys[SDLK_UP] then
					curitem = curitem - 1
					itemticks = ticks + ntickdelay
					if curitem = 4 then curitem = 9
					if curitem = -1 then curitem = 4
				end if
				if keys[SDLK_DOWN] then
					curitem = curitem + 1
					itemticks = ticks + ntickdelay
					if curitem = 5 then curitem = 0
					if curitem = 10 then curitem = 5
				end if
				if keys[SDLK_LEFT] then
					curitem = curitem - 5
					itemticks = ticks + ntickdelay
				end if
				if keys[SDLK_RIGHT] then
					curitem = curitem + 5
					itemticks = ticks + ntickdelay
				end if


				if curitem > 9 then curitem = curitem - 10
				if curitem < 0 then curitem = curitem + 10
			end if
		end if

	end if
end sub


sub game_checktrigger ()


	npx = player.px + 12
	npy = player.py + 20

	lx = (npx - (npx MOD 16)) / 16
	ly = (npy - (npy MOD 16)) / 16

	canusekey = 0

	if triggerloc(lx, ly) > -1 then game_processtrigger triggerloc(lx, ly)

end sub


sub game_configmenu ()

	dim configwindow as SDL_Surface ptr




	cursel = 0

	tickwait = 100
	keypause = ticks + tickwait

	configwindow = SDL_DisplayFormat(videobuffer)

	configwindow = IMG_Load("art/configwindow.bmp")
	t = SDL_SetColorKey(configwindow, SDL_SRCCOLORKEY, SDL_MapRGB(configwindow->format, 255, 0, 255))
	SDL_SetAlpha configwindow, 0 OR SDL_SRCALPHA, 160

	dim fil$(20)

	ticks1 = ticks
	do

		rc.x = 0
		rc.y = 0

		rc2.x = SCR_TOPX
		rc2.y = SCR_TOPY
		rc2.w = 320
		rc2.h = 240



		SDL_FillRect videobuffer, null, 0


		rcDest.x = 256 + 256 * cos(3.141592 / 180 * clouddeg * 40)
		rcDest.y = 192 + 192 * sin(3.141592 / 180 * clouddeg * 40)
		rcDest.w = 320
		rcDest.h = 240

		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 128
		SDL_BlitSurface cloudimg, @rcDest, videobuffer, null
		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 64


		rcDest.x = 256 '+ 256 * cos(3.141592 / 180 * clouddeg)
		rcDest.y = 192 '+ 192 * sin(3.141592 / 180 * clouddeg)
		rcDest.w = 320
		rcDest.h = 240

		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 128
		SDL_BlitSurface cloudimg, @rcDest, videobuffer, null
		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 64


		SDL_BlitSurface configwindow, null, videobuffer, null

		sy = 38 + (240 - 38) / 2 - 88

		for i = 0 to 21
			vr$ = ""
			vl$ = ""
			if i = 0 then vr$ = "Resolution:"
			if i = 2 then vr$ = "Bit Depth:"
			if i = 6 then vr$ = "Start Fullscreen:"
			if i = 9 then vr$ = "Music:"
			if i = 12 then vr$ = "Sound Effects:"
			if i = 15 then vr$ = "Music Volume:"
			if i = 17 then vr$ = "Effects Volume:"

			if i = 0 then vl$ = "320x240"
			if i = 1 then vl$ = "640x480"

			if i = 2 then vl$ = "16"
			if i = 3 then vl$ = "24"
			if i = 4 then vl$ = "32"

			if i = 6 then vl$ = "Yes"
			if i = 7 then vl$ = "No"

			if i = 9 then vl$ = "On"
			if i = 10 then vl$ = "Off"

			if i = 12 then vl$ = "On"
			if i = 13 then vl$ = "Off"

			lo = opmusicvol / 255 * 9
			if lo < 0 then lo = 0
			if lo > 9 then lo = 9
			la = lo
			lb = 9 - lo

			if i = 15 then vl$ = "[" + string$(la,"-") + "X" + string$(lb,"-") + "]"

			lo = opeffectsvol / 255 * 9
			if lo < 0 then lo = 0
			if lo > 9 then lo = 9
			la = lo
			lb = 9 - lo

			if i = 17 then vl$ = "[" + string$(la,"-") + "X" + string$(lb,"-") + "]"

			if i = 19 then vl$ = "Exit + Save"
			if i = 21 then vl$ = "Exit"

			cl = 3
			if i = 0 and SCR_WIDTH = 320 then cl = 0
			if i = 1 and SCR_WIDTH = 640 then cl = 0
			if i = 2 and SCR_BITS = 16 then cl = 0
			if i = 3 and SCR_BITS = 24 then cl = 0
			if i = 4 and SCR_BITS = 32 then cl = 0
			if i = 6 and opfullscreen <> 0 then cl = 0
			if i = 7 and opfullscreen = 0 then cl = 0
			if i = 9 and opmusic = 1 then cl = 0
			if i = 10 and opmusic = 0 then cl = 0
			if i = 12 and opeffects = 1 then cl = 0
			if i = 13 and opeffects = 0 then cl = 0

			if i > 18 then cl = 0

			sys_print videobuffer, vr$, 156 - 8 * len(vr$), sy + i * 8, 0
			sys_print videobuffer, vl$, 164, sy + i * 8, cl
		next

		y = 156
		x = 160 - 9 * 4
		'sys_print videobuffer, "new game/save/load", x, y, 4
		'sys_print videobuffer, "options", x, y + 16, 4
		'sys_print videobuffer, "quit game", x, y + 32, 4

		curselt = cursel
		if cursel > 4 then curselt = curselt + 1
		if cursel > 6 then curselt = curselt + 1
		if cursel > 8 then curselt = curselt + 1
		if cursel > 10 then curselt = curselt + 1
		if cursel > 11 then curselt = curselt + 1
		if cursel > 12 then curselt = curselt + 1
		if cursel > 13 then curselt = curselt + 1

		rc.x = 148 + 3 * cos(3.14159 * 2 * itemyloc / 16)
		rc.y = sy + 8 * curselt - 4

		SDL_BlitSurface itemimg(15), null, videobuffer, @rc

		rc.x = 0
		rc.y = 0
		rc.w = 320
		rc.h = 240

		yy = 255
		if ticks < ticks1 + 1000 then
			yy = 255 * ((ticks - ticks1) / 1000)
			if yy < 0 then yy = 0
			if yy > 255 then yy = 255
		end if

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, yy
		SDL_BlitSurface videobuffer, @rc, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if


		itemyloc = itemyloc + .75 * fpsr
		while itemyloc >= 16: itemyloc = itemyloc - 16: wend


		if keypause < ticks then
			SDL_PollEvent(@event)
			keys = SDL_GetKeyState(null)

			if event.type = SDL_KEYDOWN then
				keypause = ticks + tickwait

				if keys[SDLK_ESCAPE] then exit do
				if cursel = 11 or cursel = 12 then
					la = FSOUND_GetVolume (0)
					lb = FSOUND_GetVolume (1)

					if keys[SDLK_LEFT] then
						if cursel = 11 then
							opmusicvol = opmusicvol - 25
							if opmusicvol < 0 then opmusicvol = 0

							FSOUND_SetVolume (musicchannel, opmusicvol)
							FSOUND_SetVolume (menuchannel, opmusicvol)

						elseif cursel = 12 then
							opeffectsvol = opeffectsvol - 25
							if opeffectsvol < 0 then opeffectsvol = 0

							FSOUND_SetVolume (FSOUND_ALL, opeffectsvol)
							FSOUND_SetVolume (musicchannel, opmusicvol)
							FSOUND_SetVolume (menuchannel, opmusicvol)

							if menabled = 1 and opeffects = 1 then
								a = FSOUND_PlaySound(FSOUND_FREE, sfx(snddoor))
								FSOUND_SetVolume (a, opeffectsvol)
							end if


						end if
					end if
					if keys[SDLK_RIGHT] then
						if cursel = 11 then
							opmusicvol = opmusicvol + 25
							if opmusicvol > 255 then opmusicvol = 255

							FSOUND_SetVolume (musicchannel, opmusicvol)
							FSOUND_SetVolume (menuchannel, opmusicvol)

						elseif cursel = 12 then
							opeffectsvol = opeffectsvol + 25
							if opeffectsvol > 255 then opeffectsvol = 255

							FSOUND_SetVolume (FSOUND_ALL, opeffectsvol)
							FSOUND_SetVolume (musicchannel, opmusicvol)
							FSOUND_SetVolume (menuchannel, opmusicvol)

							if menabled = 1 and opeffects = 1 then
								a = FSOUND_PlaySound(FSOUND_FREE, sfx(snddoor))
								FSOUND_SetVolume (a, opeffectsvol)
							end if

						end if
					end if
				end if

				if keys[SDLK_UP] then cursel = cursel - 1
				if keys[SDLK_DOWN] then cursel = cursel + 1
				if keys[SDLK_SPACE] or keys[SDLK_RETURN] then
					if cursel = 0 then
						fullscreen = opfullscreen OR HWACCEL OR HWSURFACE

						video = SDL_SetVideoMode(320, 240, SCR_BITS, fullscreen)
						if video = 0 then
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)
						else
							SCR_WIDTH = 320
							SCR_HEIGHT = 240
							SCR_TOPX = 0
							SCR_TOPY = 0
						end if

						SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
					end if
					if cursel = 1 then
						fullscreen = opfullscreen OR HWACCEL OR HWSURFACE

						video = SDL_SetVideoMode(640, 480, SCR_BITS, fullscreen)
						if video = 0 then
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)
						else
							SCR_WIDTH = 640
							SCR_HEIGHT = 480
							SCR_TOPX = 160
							SCR_TOPY = 120
						end if

						SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
					end if
					if cursel = 2 or cursel = 3 or cursel = 4 then
						fullscreen = opfullscreen OR HWACCEL OR HWSURFACE

						b = 16
						if cursel = 3 then b = 24
						if cursel = 4 then b = 32
						video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, b, fullscreen)
						if video = 0 then
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)
						else
							SCR_BITS = b
						end if

						SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
					end if
					if cursel = 5 then
						ofullscreen = opfullscreen OR HWACCEL OR HWSURFACE
						fullscreen = SDL_FULLSCREEN OR HWACCEL OR HWSURFACE

						video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)
						if video = 0 then
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, ofullscreen)
						else
							opfullscreen = SDL_FULLSCREEN
						end if

						SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
					end if
					if cursel = 6 then
						ofullscreen = opfullscreen OR HWACCEL OR HWSURFACE
						fullscreen = 0 OR HWACCEL OR HWSURFACE

						video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)
						if video = 0 then
							video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, ofullscreen)
						else
							opfullscreen = 0
						end if

						SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
					end if
					if cursel = 7 and opmusic = 0 then
						opmusic = 1
						if menabled = 1 then
							menuchannel = FSOUND_playSound(FSOUND_FREE, mmenu)
							FSOUND_SetVolume (menuchannel, opmusicvol)
						end if
					end if
					if cursel = 8 and opmusic = 1 then
						opmusic = 0
						FSOUND_StopSound(musicchannel)
						FSOUND_StopSound(menuchannel)
					end if
					if cursel = 9 and opeffects = 0 then
						opeffects = 1
						if menabled = 1 then
							a = FSOUND_PlaySound(FSOUND_FREE, sfx(snddoor))
							FSOUND_SetVolume (a, opeffectsvol)
						end if
					end if

					if cursel = 10 and opeffects = 1 then opeffects = 0

					if cursel = 13 then
						open "config.ini" for input as #1
							for i = 0 to 14
								input #1, fil$(i)
							next
						close #1
						open "config.ini" for output as #1
							print #1, fil$(0)
							print #1, SCR_WIDTH
							print #1, fil$(2)
							print #1, SCR_HEIGHT
							print #1, fil$(4)
							print #1, SCR_BITS
							print #1, fil$(6)
							print #1, fil$(7)
							if opfullscreen = 0 then print #1, "FULLSCREEN:NO"
							if opfullscreen <> 0 then print #1, "FULLSCREEN:YES"
							if opmusic = 0 then print #1, "MUSIC:NO"
							if opmusic = 1 then print #1, "MUSIC:YES"
							if opeffects = 0 then print #1, "SNDEFFECTS:NO"
							if opeffects = 1 then print #1, "SNDEFFECTS:YES"
							print #1, fil$(11)
							print #1, opmusicvol
							print #1, fil$(13)
							print #1, opeffectsvol
						close #1

						exit do
					end if

					if cursel = 14 then exit do
				end if

				if cursel = -1 then cursel = 14
				if cursel = 15 then cursel = 0
			end if
		end if

		clouddeg = clouddeg + .01 * fpsr
		while clouddeg >= 360: clouddeg = clouddeg - 360: wend

		sleep 2
	loop

		SDL_FreeSurface configwindow

		itemticks = ticks + 210

		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 64


end sub


sub game_damagenpc (npcnum, damage, spell)


	dim npx as single, npy as single

	if damage = 0 then
		t$ = "miss!"
		fcol = 2
	else

		ratio! = 0
		heal = 0
		if damage < 0 then heal = 1
		if damage < 0 then damage = -damage

		if heal = 0 then
			if damage > npcinfo(npcnum).hp then
				ratio! = (damage - npcinfo(npcnum).hp) / damage
				damage = npcinfo(npcnum).hp
			end if
		else
			if damage > npcinfo(npcnum).maxhp - npcinfo(npcnum).hp then damage = npcinfo(npcnum).maxhp - npcinfo(npcnum).hp
		end if

		npcinfo(npcnum).pause = ticks + 900
		if npcinfo(npcnum).spriteset = 11 then npcinfo(npcnum).pause = ticks + 900

		if heal = 0 then npcinfo(npcnum).hp = npcinfo(npcnum).hp - damage
		if heal = 1 then npcinfo(npcnum).hp = npcinfo(npcnum).hp + damage

		if npcinfo(npcnum).hp > npcinfo(npcnum).maxhp then npcinfo(npcnum).hp = npcinfo(npcnum).maxhp

		t$ = "-"+ltrim$(rtrim$(str$(damage)))
		fcol = 1
		if spell = 0 then player.attackstrength = ratio! * 100

		if heal = 1 then
			t$ = "+"+ltrim$(rtrim$(str$(damage)))
			fcol = 5
		end if


	end if
	game_addFloatText t$, npcinfo(npcnum).x + 12 - 4 * len(t$), npcinfo(npcnum).y + 16, fcol


	if npcinfo(npcnum).spriteset = 12 then game_castspell 9, npcinfo(npcnum).x, npcinfo(npcnum).y, player.px, player.py, npcnum

	if npcinfo(npcnum).hp < 0 then npcinfo(npcnum).hp = 0

	if npcinfo(npcnum).hp = 0 then

		player.exp = player.exp + npcinfo(npcnum).maxhp

		if npcinfo(npcnum).spriteset = 1 or npcinfo(npcnum).spriteset = 7 or npcinfo(npcnum).spriteset = 6 then
			ff = int(rnd * player.level * 3)
			if ff = 0 then

				npx = npcinfo(npcnum).x + 12
				npy = npcinfo(npcnum).y + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if objmap(lx, ly) = -1 then objmap(lx, ly) = 4
			end if
		end if


		if npcinfo(npcnum).spriteset = 2 or npcinfo(npcnum).spriteset = 9 or npcinfo(npcnum).spriteset = 4 or npcinfo(npcnum).spriteset = 5 then
			ff = int(rnd * player.level)
			if ff = 0 then

				npx = npcinfo(npcnum).x + 12
				npy = npcinfo(npcnum).y + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if objmap(lx, ly) = -1 then objmap(lx, ly) = 12
			end if
		end if


		if npcinfo(npcnum).spriteset = 9 or npcinfo(npcnum).spriteset = 10 or npcinfo(npcnum).spriteset = 5 then
			ff = int(rnd * player.level * 2)
			if ff = 0 then

				npx = npcinfo(npcnum).x + 12
				npy = npcinfo(npcnum).y + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if objmap(lx, ly) = -1 then objmap(lx, ly) = 13
			end if
		end if

		'academy master key chest script
		if npcinfo(npcnum).script = 2 then

			cx = 9
			cy = 7

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 5

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				scriptflag (2, 0) = 1


			end if
		end if

		'academy crystal chest script
		if npcinfo(npcnum).script = 3 then

			cx = 9
			cy = 7

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 6

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16


				scriptflag (3, 0) = 1

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

			end if
		end if


		'tower shield chest script
		if npcinfo(npcnum).script = 4 and scriptflag(4,0) = 0 then


				triggerloc(9,7) = 5004

				curtile = 40
				curtilel = 0
				curtilex = curtile MOD 20
				curtiley = (curtile - curtilex) / 20

				tileinfo (l, 9, 7, 0) = curtile + 1
				tileinfo (l, 9, 7, 1) = 0

				rcSrc.x = curtilex * 16
				rcSrc.y = curtiley * 16
				rcSrc.w = 16
				rcSrc.h = 16

				rcDest.x = 9 * 16
				rcDest.y = 7 * 16
				rcDest.w = 16
				rcDest.h = 16

				SDL_BlitSurface tiles(curtilel), @rcSrc, mapbg, @rcDest

		end if

		'firehydra sword chest
		if npcinfo(npcnum).script = 5 then

			cx = 9
			cy = 6

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 9

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16


				scriptflag (5, 0) = 1

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

			end if

		end if

		'gardens master key script
		if npcinfo(npcnum).script = 8 and scriptflag(6, 0) = 0 then

			cx = 13
			cy = 7

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 5

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				scriptflag (8, 0) = 1


			end if
		end if

		'regular key chest 1

		for s = 20 to 23
			if npcinfo(npcnum).script = s and scriptflag(s, 0) < 2 then

				cx = 9
				cy = 7

				alive = 0
				for i = 1 to lastnpc
					if npcinfo(i).hp > 0 then alive = 1
				next

				if alive = 0 then

					objmap(cx, cy) = 11

					rcDest.x = cx * 8
					rcDest.y = cy * 8
					rcDest.w = 8
					rcDest.h = 8

					npx = player.px + 12
					npy = player.py + 20

					lx = (npx - (npx MOD 16)) / 16
					ly = (npy - (npy MOD 16)) / 16

					if lx = cx and ly = cy then player.py = player.py + 16

					scriptflag(s, 0) = 1

					t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				end if
			end if
		next

		'pickup lightning bomb
		if npcinfo(npcnum).script = 9 and (curmap = 41 and scriptflag(9, 1) = 0) then

			cx = 9
			cy = 7

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 13

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

			end if
		end if



		'citadel armour chest
		if npcinfo(npcnum).script = 12 then

			cx = 8
			cy = 7

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 16

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16


				scriptflag (12, 0) = 1

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

			end if
		end if

		'citadel master key script
		if npcinfo(npcnum).script = 13 and scriptflag(13, 0) = 0 then

			cx = 11
			cy = 10

			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				objmap(cx, cy) = 5

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				scriptflag (13, 0) = 1


			end if
		end if

		'max ups
		if npcinfo(npcnum).script = 15 and scriptflag(15, 0) = 0 then


			alive = 0
			for i = 1 to lastnpc
				if npcinfo(i).hp > 0 then alive = 1
			next

			if alive = 0 then

				cx = 6
				cy = 8

				objmap(cx, cy) = 18

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				scriptflag (15, 0) = 1


				cx = 9
				cy = 8

				objmap(cx, cy) = 19

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				scriptflag (16, 0) = 1

				cx = 12
				cy = 8

				objmap(cx, cy) = 20

				rcDest.x = cx * 8
				rcDest.y = cy * 8
				rcDest.w = 8
				rcDest.h = 8

				npx = player.px + 12
				npy = player.py + 20

				lx = (npx - (npx MOD 16)) / 16
				ly = (npy - (npy MOD 16)) / 16

				if lx = cx and ly = cy then player.py = player.py + 16

				t = SDL_FillRect(clipbg2, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

				scriptflag (17, 0) = 1

			end if
		end if

		if npcinfo(npcnum).script = 14 then game_endofgame

	end if


end sub

sub game_damageplayer (damage)

	d! = damage

	for ff = 1 to player.level
		d! = d! * .9
	next

	damage = d!
	player.hp = player.hp - damage
	if player.hp < 0 then player.hp = 0

	t$ = "-" + ltrim$(rtrim$(str$(damage)))
	if damage = 0 then t$ = "miss!"

	game_addFloatText t$, player.px + 12 - 4 * len(t$), player.py + 16, 4

	player.pause = ticks + 1000

end sub

SUB game_drawanims (Layer)



	FOR sx = 0 TO 19

		FOR sy = 0 TO 14
			o = objmap(sx, sy)


			IF o > -1 THEN

				xtiles = objectinfo(o, 1)
				ytiles = objectinfo(o, 2)
				cframe = objectframe(o, 1)

				FOR x = 0 TO xtiles - 1
					FOR y = 0 TO ytiles - 1

						x1 = (sx + x) * 16
						y1 = (sy + y) * 16

						IF objecttile(o, cframe, x, y, 1) = Layer THEN

							c = objecttile(o, cframe, x, y, 0)
							c = c - 1
							curtilel = 3
							curtilex = c MOD 20
							curtiley = (c - curtilex) / 20

							if curmap = 58 and scriptflag(60,0) > 0 then curtilex = 1
							if curmap = 54 and scriptflag(60,0) > 1 then curtilex = 1
							rcSrc.x = curtilex * 16
							rcSrc.y = curtiley * 16
							rcSrc.w = 16
							rcSrc.h = 16

							rcDest.x = x1
							rcDest.y = y1
							rcDest.w = 16
							rcDest.h = 16

							SDL_BlitSurface tiles(curtilel), @rcSrc, videobuffer, @rcDest

						END IF

						IF Layer = 1 THEN

							FOR l = 1 TO 2

								c = tileinfo(l, sx + x, sy + y, 0)
								IF c > 0 THEN

									cl = tileinfo(l, sx + x, sy + y, 1)

									c = c - 1
									curtile = c
									curtilel = cl
									curtilex = c MOD 20
									curtiley = (c - curtilex) / 20

									rcSrc.x = curtilex * 16
									rcSrc.y = curtiley * 16
									rcSrc.w = 16
									rcSrc.h = 16

									rcDest.x = (sx + x) * 16
									rcDest.y = (sy + y) * 16
									rcDest.w = 16
									rcDest.h = 16

									pass = 1
									if curtilel = 1 then
										for ff = 0 to 5
											ffa = 20 * 5 - 1 + ff * 20
											ffb = 20 * 5 + 4 + ff * 20
											if curtile > ffa and curtile < ffb then pass = 0
										next
									end if


									if pass = 1 then SDL_BlitSurface tiles(curtilel), @rcSrc, videobuffer, @rcDest

								END IF
							NEXT
						END IF
					NEXT
				NEXT
			END IF

		NEXT
	NEXT


end sub



sub game_drawhud ()

	'sys_print videobuffer, str$(fps) + str$(curmap) + str$(scriptflag(60,0)), 0, 0, 0



	dim ccc as long

	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 320
	rcSrc.h = 240

	SDL_FillRect videobuffer2, @rcSrc, 0

	for i = 0 to maxfloat
		if floattext(i, 0) > 0 then
			fc = int(floattext(i, 3))

			if fc = 1 or fc = 3 then
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) - 1, 2
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) + 1, 2
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) - 1, int(floattext(i, 2)) + 0, 2
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 1, int(floattext(i, 2)) + 0, 2
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)), int(floattext(i, 2)), fc
			end if
			if fc = 2 then
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) - 1, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) + 1, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) - 1, int(floattext(i, 2)) + 0, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 1, int(floattext(i, 2)) + 0, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)), int(floattext(i, 2)), fc
			end if

			if fc = 4 then
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) - 1, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) + 1, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) - 1, int(floattext(i, 2)) + 0, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 1, int(floattext(i, 2)) + 0, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)), int(floattext(i, 2)), 1
			end if

			if fc = 5 then
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) - 1, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 0, int(floattext(i, 2)) + 1, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) - 1, int(floattext(i, 2)) + 0, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)) + 1, int(floattext(i, 2)) + 0, 3
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)), int(floattext(i, 2)), 0
			end if


			if fc = 0 then
				sys_print videobuffer, floatstri$(i), int(floattext(i, 1)), int(floattext(i, 2)), fc
			end if
		end if

		if floaticon(i, 0) > 0 then
			ico = floaticon(i, 3)
			ix = floaticon(i, 1)
			iy = floaticon(i, 2)


			rcDest.x = ix
			rcDest.y = iy

			if ico <> 99 then SDL_BlitSurface itemimg(ico), null, videobuffer, @rcDest
			if ico = 99 then

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, int(rnd * 96) + 96

				rcSrc.x = 16 * int(rnd * 2)
				rcSrc.y = 80
				rcSrc.w = 16
				rcSrc.h = 16

				rcDest.x = ix
				rcDest.y = iy

				SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

				SDL_SetAlpha spellimg, 0 OR SDL_SCRALPHA, 255
			end if
		end if
	next


	if itemselon = 0 then


	sy = 211


	nx = 19 * 8 + 13
	rcSrc.x = nx - 17 + 48
	rcSrc.y = sy

	if player.foundcrystal = 1 then
		rcSrc.x = rcSrc.x + 17

		SDL_BlitSurface itemimg(7), null, videobuffer, @rcSrc

		ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

		rcSrc2.x = rcSrc.x
		rcSrc2.y = sy + 16
		rcSrc2.w = 16
		rcSrc2.h = 4

		SDL_FillRect videobuffer, @rcSrc2, ccc

		ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64)
		if player.crystalcharge = 100 then ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)

		mx = player.crystalcharge / 100 * 14
		if mx > 14 then mx = 14

		rcSrc2.x = rcSrc.x + 1
		rcSrc2.y = sy + 17
		rcSrc2.w = mx
		rcSrc2.h = 2

		SDL_FillRect videobuffer, @rcSrc2, ccc


		for i = 0 to 4
			rcSrc.x = rcSrc.x + 17

			if player.foundspell(i) = 1 then

				SDL_BlitSurface itemimg(8 + i), null, videobuffer, @rcSrc

				ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

				rcSrc2.x = rcSrc.x
				rcSrc2.y = sy + 16
				rcSrc2.w = 16
				rcSrc2.h = 4

				SDL_FillRect videobuffer, @rcSrc2, ccc

				ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64)
				if player.spellcharge(i) = 100 then ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)

				mx = player.spellcharge(i) / 100 * 14
				if mx > 14 then mx = 14

				rcSrc2.x = rcSrc.x + 1
				rcSrc2.y = sy + 17
				rcSrc2.w = mx
				rcSrc2.h = 2

				SDL_FillRect videobuffer, @rcSrc2, ccc
			end if


		next
	end if

		exit sub
	end if


	if selenemyon = 0 then

		rcDest.x = 0
		rcDest.y = 0
		rcDest.w = 320
		rcDest.h = 240

		rc2.x = 0
		rc2.y = 0


		SDL_SetAlpha videobuffer2, 0 OR SDL_SRCALPHA, player.itemselshade * 4

		SDL_FillRect videobuffer2, @rcDest, 0

		SDL_BlitSurface videobuffer2, null, videobuffer, @rc2



		sy = 202

		rcSrc.x = 46
		rcSrc.y = 46

		SDL_SetAlpha inventoryimg, 0 OR SDL_SRCALPHA, 160'128
		SDL_BlitSurface inventoryimg, null, videobuffer, @rcSrc
		SDL_SetAlpha inventoryimg, 0 OR SDL_SRCALPHA, 255

		sx = 54
		sy = 55

		'draw map 9,77
		rcDest.x = 46 + 9
		rcDest.y = 46 + 77

		amap = 0
		if curmap > 46 then amap = 2
		if curmap > 67 then amap = 3
		if curmap > 5 and curmap < 42 then amap = 1
		SDL_BlitSurface mapimg(amap), null, videobuffer, @rcDest

		ccc = SDL_MapRGB(videobuffer->format, 128 + 127 * sin(3.141592 * 2 * itemyloc / 16), 0, 0)

		for b = 0 to 6
			for a = 0 to 12
				if invmap(amap, a, b) = curmap then
					rcSrc2.x = 46 + 9 + a * 9 + 2
					rcSrc2.y = 46 + 77 + b * 9 + 1
					rcSrc2.w = 6
					rcSrc2.h = 6
				end if
			next
		next


		SDL_FillRect videobuffer, @rcSrc2, ccc

		if amap = 1 then
			sys_print videobuffer, "L1", 46 + 9, 46 + 77, 0
			sys_print videobuffer, "L2", 46 + 9 + 7 * 9, 46 + 77, 0
		end if


		cc = 0
		if player.hp <= player.maxhp * .25 then cc = int(player.hpflash)

		sys_print videobuffer, "Health: "+ltrim$(rtrim$(str$(player.hp))) + "/" + ltrim$(rtrim$(str$(player.maxhp))), sx, sy, cc

		f$ = ltrim$(rtrim$(str$(player.level)))
		if player.level = 22 then f$ = "MAX"
		sys_print videobuffer, "Level : "+f$, sx, sy + 8, 0

		mx = player.exp / player.nextlevel * 14
		if mx > 14 then mx = 14

		ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

		rcSrc2.x = sx + 64
		rcSrc2.y = sy + 16
		rcSrc2.w = 16
		rcSrc2.h = 4

		SDL_FillRect videobuffer, @rcSrc2, ccc

		ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64)

		rcSrc2.x = sx + 65
		rcSrc2.y = sy + 17
		rcSrc2.w = mx
		rcSrc2.h = 2

		SDL_FillRect videobuffer, @rcSrc2, ccc




		mx = player.attackstrength / 100 * 54
		if mx > 54 then mx = 54

		mx2 = player.spellstrength / 100 * 54
		if mx2 > 54 then mx2 = 54

		ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

		rcSrc2.x = sx
		rcSrc2.y = sy + 16
		rcSrc2.w = 56
		rcSrc2.h = 6

		SDL_FillRect videobuffer, @rcSrc2, ccc

		ccc = SDL_MapRGB(videobuffer->format, 0, 64, 224)
		if player.attackstrength = 100 then ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)

		rcSrc2.x = sx + 1
		rcSrc2.y = sy + 17
		rcSrc2.w = mx
		rcSrc2.h = 2

		SDL_FillRect videobuffer, @rcSrc2, ccc

		ccc = SDL_MapRGB(videobuffer->format, 128, 0, 224)
		if player.spellstrength = 100 then ccc = SDL_MapRGB(videobuffer->format, 224, 0, 0)

		rcSrc2.x = sx + 1
		rcSrc2.y = sy + 19
		rcSrc2.w = mx2
		rcSrc2.h = 2

		SDL_FillRect videobuffer, @rcSrc2, ccc

		ase = secstart + secsingame
		h = ((ase - (ase mod 3600)) / 3600)
		ase = (ase - h * 3600)
		m = ((ase - (ase mod 60)) / 60)
		s = (ase - m * 60)

		h$ = ltrim$(rtrim$(str$(h)))
		if len(h$) = 1 then h$ = "0" + h$
		m$ = ltrim$(rtrim$(str$(m)))
		if len(m$) = 1 then m$ = "0" + m$
		s$ = ltrim$(rtrim$(str$(s)))
		if len(s$) = 1 then s$ = "0" + s$

		t$ = h$ + ":" + m$ + ":" + s$
		sys_print videobuffer, t$, 46 + 38 - len(t$) * 4, 46 + 49, 0


		sys_print videobuffer, "Use", 193, 55, 0
		sys_print videobuffer, "Cast", 236, 55, 0


		rcSrc.x = 128
		rcSrc.y = 91

		ss = (player.sword - 1) * 3
		if player.sword = 3 then ss = 18
		SDL_BlitSurface itemimg(ss), null, videobuffer, @rcSrc

		rcSrc.x = rcSrc.x + 16
		ss = (player.shield - 1) * 3 + 1
		if player.shield = 3 then ss = 19
		SDL_BlitSurface itemimg(ss), null, videobuffer, @rcSrc

		rcSrc.x = rcSrc.x + 16
		ss = (player.armour - 1) * 3 + 2
		if player.armour = 3 then ss = 20
		SDL_BlitSurface itemimg(ss), null, videobuffer, @rcSrc

		for i = 0 to 4
			sx = 188
			sy = 70 + i * 24
			rcSrc.x = sx
			rcSrc.y = sy
			if i = 0 then SDL_BlitSurface itemimg(6), null, videobuffer, @rcSrc
			if i = 1 then SDL_BlitSurface itemimg(12), null, videobuffer, @rcSrc
			if i = 2 then SDL_BlitSurface itemimg(17), null, videobuffer, @rcSrc
			if i = 3 then SDL_BlitSurface itemimg(16), null, videobuffer, @rcSrc
			if i = 4 then SDL_BlitSurface itemimg(14), null, videobuffer, @rcSrc

			sys_print videobuffer, "x"+ltrim$(rtrim$(str$(inventory(i)))), sx + 17, sy + 7, 0

		next

		if player.foundcrystal = 1 then
			rcSrc.x = 243
			rcSrc.y = 67
			sy = 67

			SDL_BlitSurface itemimg(7), null, videobuffer, @rcSrc

			ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

			rcSrc2.x = rcSrc.x
			rcSrc2.y = sy + 16
			rcSrc2.w = 16
			rcSrc2.h = 4

			SDL_FillRect videobuffer, @rcSrc2, ccc

			ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64)
			if player.crystalcharge = 100 then ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)

			mx = player.crystalcharge / 100 * 14
			if mx > 14 then mx = 14

			rcSrc2.x = rcSrc.x + 1
			rcSrc2.y = sy + 17
			rcSrc2.w = mx
			rcSrc2.h = 2

			SDL_FillRect videobuffer, @rcSrc2, ccc


			for i = 0 to 3
				rcSrc.x = 243
				rcSrc.y = 91 + i * 24
				sy = rcSrc.y

				if player.foundspell(i) = 1 then

					SDL_BlitSurface itemimg(8 + i), null, videobuffer, @rcSrc

					ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

					rcSrc2.x = rcSrc.x
					rcSrc2.y = sy + 16
					rcSrc2.w = 16
					rcSrc2.h = 4

					SDL_FillRect videobuffer, @rcSrc2, ccc

					ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64)
					if player.spellcharge(i) = 100 then ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)

					mx = player.spellcharge(i) / 100 * 14
					if mx > 14 then mx = 14

					rcSrc2.x = rcSrc.x + 1
					rcSrc2.y = sy + 17
					rcSrc2.w = mx
					rcSrc2.h = 2

					SDL_FillRect videobuffer, @rcSrc2, ccc
				end if


			next
		end if

		if itemselon = 1 then
			for i = 0 to 4
				if curitem = 5 + i then
					rcDest.x = 243 - 12 + 3 * sin(3.141592 * 2 * itemyloc / 16)
					rcDest.y = 67 + 24 * i
					SDL_Blitsurface itemimg(15), null, videobuffer, @rcDest
				end if

				if curitem = i then
					rcDest.x = 189 - 12 + 3 * sin(3.141592 * 2 * itemyloc / 16)
					rcDest.y = 70 + 24 * i
					SDL_Blitsurface itemimg(15), null, videobuffer, @rcDest
				end if
			next
		end if

	end if

	if selenemyon = 1 then
		if curenemy > lastnpc then
			pst = curenemy - lastnpc - 1
			rcDest.x = postinfo(pst, 0)
			rcDest.y = postinfo(pst, 1) - 4 - sin(3.141592 / 8 * itemyloc)
		else
			rcDest.x = npcinfo(curenemy).x + 4
			rcDest.y = npcinfo(curenemy).y + 4 - 16 - sin(3.141592 / 8 * itemyloc)
		end if

		SDL_Blitsurface itemimg(13), null, videobuffer, @rcDest
	end if




end sub


SUB game_drawnpcs (mode)

	dim ccc as uinteger


	ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)

	lok = 0
	if mode = 1 then lok = 1

	fst = firsty
	lst = lasty

	if mode = 0 then lst = player.ysort
	if mode = 1 then fst = player.ysort

	for yy = fst to lst

		if ysort(yy) > 0 then
			i = ysort(yy)

			IF npcinfo(i).hp > 0 THEN

				npx = int(npcinfo(i).x)
				npy = int(npcinfo(i).y)

				'IF (npy <= player.py AND mode = 0) OR (npy > player.py AND mode = 1) THEN

					sprite = npcinfo(i).spriteset

					wdir = npcinfo(i).walkdir

					'spriteset1 specific
					if npcinfo(i).spriteset = 1 then

						if npcinfo(i).attacking = 0 then

							cframe = npcinfo(i).cframe

							rcSrc.x = int(cframe / 4) * 24
							rcSrc.y = wdir * 24
							rcSrc.w = 24
							rcSrc.h = 24

							rcDest.x = npx
							rcDest.y = npy
							rcDest.w = 24
							rcDest.h = 24

							if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
								npcinfo(i).shake = ticks + 50
								rcDest.x = rcDest.x + int(rnd * 3) - 1
								rcDest.y = rcDest.y + int(rnd * 3) - 1
							end if

							SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest
						else


							cframe = npcinfo(i).cattackframe


							rcSrc.x = int(cframe / 4) * 24
							rcSrc.y = wdir * 24
							rcSrc.w = 24
							rcSrc.h = 24

							rcDest.x = npx
							rcDest.y = npy
							rcDest.w = 24
							rcDest.h = 24

							SDL_BlitSurface animsa(sprite), @rcSrc, videobuffer, @rcDest
						end if

					end if


					'onewing
					if npcinfo(i).spriteset = 2 then
						for f = 0 to 7

							s = npcinfo(i).bodysection(f).sprite
							rcSrc.x = animset2(s).x
							rcSrc.y = animset2(s).y
							rcSrc.w = animset2(s).w
							rcSrc.h = animset2(s).h

							rcDest.x = npcinfo(i).bodysection(f).x - animset2(s).xofs
							rcDest.y = npcinfo(i).bodysection(f).y - animset2(s).yofs + 2

							SDL_BlitSurface anims(2), @rcSrc, videobuffer, @rcDest

						next

					end if


					'twowing
					if npcinfo(i).spriteset = 9 then
						for f = 0 to 7

							yp = 0

							if f = 0 and (curmap = 53 or curmap = 57 or curmap = 61 or curmap = 65 or curmap = 56 or curmap > 66) and scriptflag(60,0) > 0 then yp = 16
							s = npcinfo(i).bodysection(f).sprite
							rcSrc.x = animset9(s).x
							rcSrc.y = animset9(s).y + yp
							rcSrc.w = animset9(s).w
							rcSrc.h = animset9(s).h

							rcDest.x = npcinfo(i).bodysection(f).x - animset9(s).xofs
							rcDest.y = npcinfo(i).bodysection(f).y - animset9(s).yofs + 2

							SDL_BlitSurface anims(9), @rcSrc, videobuffer, @rcDest

						next

					end if


					' boss 1
					if npcinfo(i).spriteset = 3 then

						if npcinfo(i).attacking = 0 then

							cframe = npcinfo(i).cframe

							rcSrc.x = int(cframe / 4) * 24
							rcSrc.y = 0
							rcSrc.w = 24
							rcSrc.h = 48

							rcDest.x = npx - 2
							rcDest.y = npy - 24

							SDL_BlitSurface anims(3), @rcSrc, videobuffer, @rcDest

						else

							rcSrc.x = 4 * 24
							rcSrc.y = 0
							rcSrc.w = 24
							rcSrc.h = 48

							rcDest.x = npx - 2
							rcDest.y = npy - 24

							SDL_BlitSurface anims(3), @rcSrc, videobuffer, @rcDest
						end if

					end if

					'black knight
					if npcinfo(i).spriteset = 4 then

						if npcinfo(i).attacking = 0 then

							cframe = npcinfo(i).cframe

							rcSrc.x = int(cframe / 4) * 24
							rcSrc.y = 0
							rcSrc.w = 24
							rcSrc.h = 48

							rcDest.x = npx - 2
							rcDest.y = npy - 24

							SDL_BlitSurface anims(4), @rcSrc, videobuffer, @rcDest

						else

							rcSrc.x = 4 * 24
							rcSrc.y = 0
							rcSrc.w = 24
							rcSrc.h = 48

							rcDest.x = npx - 2
							rcDest.y = npy - 24

							SDL_BlitSurface anims(4), @rcSrc, videobuffer, @rcDest
						end if

					end if


					'firehydra
					if npcinfo(i).spriteset = 5 then
						for ff = 0 to 2

							if npcinfo(i).hp > 10 * ff * 20 then

								rcSrc.x = 16 * int(rnd * 2)
								rcSrc.y = 80
								rcSrc.w = 16
								rcSrc.h = 16

								rcDest.x = npcinfo(i).bodysection(10 * ff).x - 8
								rcDest.y = npcinfo(i).bodysection(10 * ff).y - 8

								x = 192 + ((itemyloc + ff * 5) mod 3)  * 64
								if x > 255 then x = 255
								SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, x
								SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest
								SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

								for f = 1 to 8

									rcSrc.x = 16 * int(rnd * 2)
									rcSrc.y = 80
									rcSrc.w = 16
									rcSrc.h = 16

									rcDest.x = npcinfo(i).bodysection(ff * 10 + f).x - 8 + int(rnd * 3) - 1
									rcDest.y = npcinfo(i).bodysection(ff * 10 + f).y - 8 + int(rnd * 3) - 1

									x = 192 + f mod 3 * 64
									if x > 255 then x = 255
									SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, x
									SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest
									SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

								next


								rcSrc.x = 0
								rcSrc.y = 0
								rcSrc.w = 42
								rcSrc.h = 36

								rcDest.x = npcinfo(i).bodysection(10 * ff + 9).x - 21
								rcDest.y = npcinfo(i).bodysection(10 * ff + 9).y - 21

								SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 192
								SDL_BlitSurface anims(5), @rcSrc, videobuffer, @rcDest
								SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

							end if

						next

					end if


					'red dragon
					if npcinfo(i).spriteset = 6 then


						cframe = npcinfo(i).cframe

						rcSrc.x = int(cframe / 4) * 24
						rcSrc.y = wdir * 24
						rcSrc.w = 24
						rcSrc.h = 24

						rcDest.x = npx
						rcDest.y = npy
						rcDest.w = 24
						rcDest.h = 24

						if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
							npcinfo(i).shake = ticks + 50
							rcDest.x = rcDest.x + int(rnd * 3) - 1
							rcDest.y = rcDest.y + int(rnd * 3) - 1
						end if

						SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest

					end if

					'wizard
					if npcinfo(i).spriteset = 7 then

						'if npcinfo(i).attacking = 0 then

							cframe = npcinfo(i).cframe

							rcSrc.x = int(cframe / 4) * 24
							rcSrc.y = wdir * 24
							rcSrc.w = 24
							rcSrc.h = 24

							rcDest.x = npx
							rcDest.y = npy
							rcDest.w = 24
							rcDest.h = 24

							if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
								npcinfo(i).shake = ticks + 50
								rcDest.x = rcDest.x + int(rnd * 3) - 1
								rcDest.y = rcDest.y + int(rnd * 3) - 1
							end if

							SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest
					'	else


							cframe = npcinfo(i).cattackframe


							rcSrc.x = int(cframe / 4) * 24
							rcSrc.y = wdir * 24
							rcSrc.w = 24
							rcSrc.h = 24

							rcDest.x = npx
							rcDest.y = npy
							rcDest.w = 24
							rcDest.h = 24

						'	SDL_BlitSurface animsa(sprite), @rcSrc, videobuffer, @rcDest
						'end if

					end if


					'yellow dragon
					if npcinfo(i).spriteset = 8 then


						cframe = npcinfo(i).cframe

						rcSrc.x = int(cframe / 4) * 24
						rcSrc.y = wdir * 24
						rcSrc.w = 24
						rcSrc.h = 24

						rcDest.x = npx
						rcDest.y = npy
						rcDest.w = 24
						rcDest.h = 24

						if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
							npcinfo(i).shake = ticks + 50
							rcDest.x = rcDest.x + int(rnd * 3) - 1
							rcDest.y = rcDest.y + int(rnd * 3) - 1
						end if

						SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest

					end if


					'dragon2
					if npcinfo(i).spriteset = 10 then

						if npcinfo(i).attacking = 0 then
							npcinfo(i).floating = npcinfo(i).floating + .25 * fpsr
							while npcinfo(i).floating >= 16: npcinfo(i).floating = npcinfo(i).floating - 16: wend


							frame! = npcinfo(i).frame
							cframe = npcinfo(i).cframe

							frame! = frame! + .5 * fpsr
							WHILE frame! >= 16
								frame! = frame! - 16
							WEND

							cframe = INT(frame!)
							IF cframe > 16 THEN cframe = 16 - 1
							IF cframe < 0 THEN cframe = 0

							npcinfo(i).frame = frame!
							npcinfo(i).cframe = cframe

							cframe = npcinfo(i).cframe

							rcSrc.x = 74 * wdir
							rcSrc.y = int(cframe / 4) * 48
							rcSrc.w = 74
							rcSrc.h = 48

							rcDest.x = npx + 12 - 37
							rcDest.y = npy + 12 - 32 - 3 * sin(3.141592 * 2 * npcinfo(i).floating / 16)
							rcDest.w = 24
							rcDest.h = 24

							if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
								npcinfo(i).shake = ticks + 50
								rcDest.x = rcDest.x + int(rnd * 3) - 1
								rcDest.y = rcDest.y + int(rnd * 3) - 1
							end if

							SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest

						else

							npcinfo(i).floating = npcinfo(i).floating + .25 * fpsr
							while npcinfo(i).floating >= 16: npcinfo(i).floating = npcinfo(i).floating - 16: wend


							cframe = npcinfo(i).cattackframe

							rcSrc.x = 74 * wdir
							rcSrc.y = int(cframe / 4) * 48
							rcSrc.w = 74
							rcSrc.h = 48

							rcDest.x = npx + 12 - 37
							rcDest.y = npy + 12 - 32 - 3 * sin(3.141592 * 2 * npcinfo(i).floating / 16)
							rcDest.w = 24
							rcDest.h = 24

							SDL_BlitSurface animsa(sprite), @rcSrc, videobuffer, @rcDest
						end if
					end if



					'end boss
					if npcinfo(i).spriteset = 11 then

						npcinfo(i).floating = npcinfo(i).floating + .3 * fpsr
						while npcinfo(i).floating >= 16: npcinfo(i).floating = npcinfo(i).floating - 16: wend


						frame! = npcinfo(i).frame2
						cframe = npcinfo(i).cframe

						frame! = frame! + .5 * fpsr
						WHILE frame! >= 16
							frame! = frame! - 16
						WEND
						npcinfo(i).frame2 = frame!

						cframe = int(frame!)

						sx = npx + 12 - 40
						sy = npy + 12 - 50 - 3 * sin(3.141592 * 2 * npcinfo(i).floating / 16)

						for fr = 0 to 3
							SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 128 + int(rnd * 96)

							rcSrc.x = 16 * int(rnd * 2)
							rcSrc.y = 80
							rcSrc.w = 16
							rcSrc.h = 16

							rcDest.x = sx + 32 + int(rnd * 3) - 1
							rcDest.y = sy - int(rnd * 6)

							SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest
						next

						FOR ii = 0 TO 8
							for i2 = 0 to 3
								rcSrc.x = 16 * int(rnd * 2)
								rcSrc.y = 80
								rcSrc.w = 16
								rcSrc.h = 16

								fr3! = frame! - 3 + i2
								if fr3! < 0 then fr3! = fr3! + 16

								rcDest.x = sx + 36 + ii * 8 - ii * COS(3.14159 * 2 * (fr3! - ii) / 16) * 2
								rcDest.y = sy + 16 + ii * SIN(3.14159 * 2 * (fr3! - ii) / 16) * 3 - ii' * 4

								SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, i2 / 3 * 224

								SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest


								xloc = rcDest.x
								yloc = rcDest.y
								xdif = (xloc + 8) - (player.px + 12)
								ydif = (yloc + 8) - (player.py + 12)

								if (abs(xdif) < 8 and abs(ydif) < 8) and player.pause < ticks then

									damage = npcinfo(i).spelldamage * (1 + rnd * .5)

									if player.hp > 0 then
										game_damageplayer damage
										if menabled = 1 and opeffects = 1 then
											 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
											 FSOUND_setVolume(snd, opeffectsvol)
										end if
									end if

								end if


								rcDest.x = sx + 36 - ii * 8 + ii * COS(3.14159 * 2 * (fr3! - ii) / 16) * 2
								rcDest.y = sy + 16 + ii * SIN(3.14159 * 2 * (fr3! - ii) / 16) * 3 - ii' * 4

								SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, i2 / 3 * 224

								SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

								xloc = rcDest.x
								yloc = rcDest.y
								xdif = (xloc + 8) - (player.px + 12)
								ydif = (yloc + 8) - (player.py + 12)

								if (abs(xdif) < 8 and abs(ydif) < 8) and player.pause < ticks then

									damage = npcinfo(i).spelldamage * (1 + rnd * .5)

									if player.hp > 0 then
										game_damageplayer damage
										if menabled = 1 and opeffects = 1 then
											 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
											 FSOUND_setVolume(snd, opeffectsvol)
										end if
									end if

								end if
							next

						NEXT


						SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

						if npcinfo(i).attacking = 0 then

							cframe = int(frame!)
							rcSrc.x = 0
							rcSrc.y = 72 * int(cframe / 4)
							rcSrc.w = 80
							rcSrc.h = 72

							rcDest.x = sx
							rcDest.y = sy

							if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
								npcinfo(i).shake = ticks + 50
								rcDest.x = rcDest.x + int(rnd * 3) - 1
								rcDest.y = rcDest.y + int(rnd * 3) - 1
							end if

							SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest
						else
							cframe = int(npcinfo(i).cattackframe)

							rcSrc.x = 0
							rcSrc.y = 72 * int(cframe / 4)
							rcSrc.w = 80
							rcSrc.h = 72

							rcDest.x = sx
							rcDest.y = sy

							SDL_BlitSurface animsa(sprite), @rcSrc, videobuffer, @rcDest
						end if

					end if

					'bat kitty
					if npcinfo(i).spriteset = 12 then

						npcinfo(i).floating = npcinfo(i).floating + 1 * fpsr
						while npcinfo(i).floating >= 16: npcinfo(i).floating = npcinfo(i).floating - 16: wend

						frame! = npcinfo(i).frame
						cframe = npcinfo(i).cframe

						frame! = frame! + .5 * fpsr
						WHILE frame! >= 16
							frame! = frame! - 16
						WEND

						cframe = INT(frame!)
						IF cframe > 16 THEN cframe = 16 - 1
						IF cframe < 0 THEN cframe = 0

						npcinfo(i).frame = frame!
						npcinfo(i).cframe = cframe

						cframe = npcinfo(i).cframe

						rcSrc.x = 0
						rcSrc.y = 0
						rcSrc.w = 99
						rcSrc.h = 80

						rcDest.x = npx + 12 - 50
						rcDest.y = npy + 12 - 64 + 2 * sin(3.141592 * 2 * npcinfo(i).floating / 16)
						rcDest.w = 99
						rcDest.h = 80

						if npcinfo(i).pause > ticks and npcinfo(i).shake < ticks then
							npcinfo(i).shake = ticks + 50
							rcDest.x = rcDest.x + int(rnd * 3) - 1
							rcDest.y = rcDest.y + int(rnd * 3) - 1
						end if

						SDL_BlitSurface anims(sprite), @rcSrc, videobuffer, @rcDest

					end if



					rcDest.x = npx + 4
					rcDest.y = npy + 22
					rcDest.w = 16
					rcDest.h = 4

					SDL_FillRect videobuffer, @rcDest, 0

					rcDest.x = npx + 5
					rcDest.y = npy + 23


					ww = 14 * npcinfo(i).hp / npcinfo(i).maxhp
					if ww > 14 then ww = 14
					if ww < 1 then ww = 1

					rcDest.w = ww
					rcDest.h = 2


					SDL_FillRect videobuffer, @rcDest, ccc

					pass = 1

					if npcinfo(i).spriteset = 3 then pass = 0
					if pass = 1 then game_drawover npx, npy

			END IF
		end if
	NEXT

END SUB


SUB game_drawover (modx, mody)


	npx = modx + 12
	npy = mody + 20

	lx = (npx - (npx MOD 16)) / 16
	ly = (npy - (npy MOD 16)) / 16

	FOR xo = -1 TO 1
		FOR yo = -1 TO 1

			sx = lx + xo
			sy = ly + yo

			sx2 = sx * 16
			sy2 = sy * 16

			IF sx > -1 AND sx < 40 AND sy > -1 AND sy < 24 THEN

				curtile = tileinfo(2, sx, sy, 0)
				curtilel = tileinfo(2, sx, sy, 1)

				IF curtile > 0 THEN
					curtile = curtile - 1
					curtilex = curtile MOD 20
					curtiley = (curtile - curtilex) / 20

					rcSrc.x = curtilex * 16
					rcSrc.y = curtiley * 16
					rcSrc.w = 16
					rcSrc.h = 16

					rcDest.x = sx2
					rcDest.y = sy2
					rcDest.w = 16
					rcDest.h = 16

					pass = 1
					if curtilel = 1 then
						for ff = 0 to 5
							ffa = 20 * 5 - 1 + ff * 20
							ffb = 20 * 5 + 4 + ff * 20
							if curtile > ffa and curtile < ffb then pass = 0
						next
					end if

					if pass = 1 then SDL_BlitSurface tiles(curtilel), @rcSrc, videobuffer, @rcDest
				END IF

			END IF

		NEXT

	NEXT


end sub


sub game_drawplayer ()

	dim ccc as long

	f = 0
	if player.armour = 3 then f = 13

	if attacking = 0 then
		rcSrc.x = int(player.walkframe / 4) * 24
		rcSrc.y = player.walkdir * 24
		rcSrc.w = 24
		rcSrc.h = 24

		rcDest.x = int(player.px)
		rcDest.y = int(player.py)
		rcDest.w = 24
		rcDest.h = 24

		SDL_BlitSurface anims(f), @rcSrc, videobuffer, @rcDest
	else
		rcSrc.x = int(player.attackframe / 4) * 24
		rcSrc.y = player.walkdir * 24
		rcSrc.w = 24
		rcSrc.h = 24

		rcDest.x = int(player.px)
		rcDest.y = int(player.py)
		rcDest.w = 24
		rcDest.h = 24

		SDL_BlitSurface animsa(f), @rcSrc, videobuffer, @rcDest

	end if

	ccc = SDL_MapRGB(videobuffer->format, 224, 224, 64)

	pass = 0
	if player.hp <= player.maxhp * .25 then pass = 1

	if pass = 1 then
		ccc = SDL_MapRGB(videobuffer->format, 255, 255, 255)
		if int(player.hpflash) = 1 then ccc = SDL_MapRGB(videobuffer->format, 255, 0, 0)
	end if

	sss= 6
	if player.foundcrystal then sss = 8
	npx = player.px
	npy = player.py
	rcDest.x = npx + 4
	rcDest.y = npy + 22
	rcDest.w = 16
	rcDest.h = sss

	SDL_FillRect videobuffer, @rcDest, 0

	rcDest.x = npx + 5
	rcDest.y = npy + 23


	ww = 14 * player.hp / player.maxhp
	if ww > 14 then ww = 14
	if ww < 1 then ww = 1

	rcDest.w = ww
	rcDest.h = 2


	SDL_FillRect videobuffer, @rcDest, ccc


	ccc = SDL_MapRGB(videobuffer->format, 0, 224, 64)
	if player.attackstrength = 100 then ccc = SDL_MapRGB(videobuffer->format, 255, 128, 32)


	ww = 14 * player.attackstrength / 100
	if ww > 14 then ww = 14

	ww2 = 14 * player.spellstrength / 100
	if ww2 > 14 then ww2 = 14

	rcDest.w = ww
	rcDest.h = 2
	rcDest.y = rcDest.y + 2

	SDL_FillRect videobuffer, @rcDest, ccc

	ccc = SDL_MapRGB(videobuffer->format, 128, 0, 224)
	if player.spellstrength = 100 then ccc = SDL_MapRGB(videobuffer->format, 224, 0, 0)

	rcDest.w = ww2
	rcDest.h = 2
	rcDest.y = rcDest.y + 2

	SDL_FillRect videobuffer, @rcDest, ccc



end sub



sub game_drawview ()



	SDL_BlitSurface mapbg, null, videobuffer, null

	game_updspellsunder

	game_drawanims 0

	'------dontdrawover = special case to make boss work right in room 24
	if dontdrawover = 1 then game_drawanims 1
	game_drawnpcs 0

	game_drawplayer


	game_drawnpcs 1
	if dontdrawover = 0 then game_drawanims 1



	game_drawover int(player.px), int(player.py)

	game_updspells

	if cloudson = 1 then
		rcDest.x = 256 + 256 * cos(3.141592 / 180 * clouddeg)
		rcDest.y = 192 + 192 * sin(3.141592 / 180 * clouddeg)
		rcDest.w = 320
		rcDest.h = 240

		SDL_BlitSurface cloudimg, @rcDest, videobuffer, null
	end if



	game_drawhud



	rcDest.x = SCR_TOPX
	rcDest.y = SCR_TOPY

	SDL_BlitSurface videobuffer, null, video, @rcDest


end sub

sub game_endofgame ()


dim xofs as single

	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 320
	rcSrc.h = 240


	rc.x = 0
	rc.y = 0
	rc.w = 320
	rc.h = 240


	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY
	rc2.w = 320
	rc2.h = 240


	x = 160 - 4 * len(stri$)


	pauseticks = ticks + 500
	bticks = ticks


	cursel = 0

	keypause = ticks + 220

	spd! = .2
	y! = 140

	ticks = SDL_GetTicks


	if menabled = 1 and opmusic = 1 then
		FSOUND_stopSound(FSOUND_ALL)
		musicchannel = FSOUND_playSound(FSOUND_FREE, mendofgame)
		FSOUND_setVolume(musicchannel, 0)
	end if

	ticks1 = ticks
	ya = 0

	SDL_FillRect videobuffer2, @rc, 0
	SDL_FillRect videobuffer3, @rc, 0
	SDL_BlitSurface videobuffer, @rc, videobuffer2, @rc

	do

		ld! = ld! + 4 * fpsr
		if ld! > opmusicvol then ld! = opmusicvol
		if menabled = 1 and ldstop = 0 then
			FSOUND_setVolume(musicchannel, ld!)
			if ld! = opmusicvol then ldstop = 1
		end if

		ya = 0
		if ticks < ticks1 + 1500 then
			ya = 255 * ((ticks - ticks1) / 1500)
			if ya < 0 then ya = 0
			if ya > 255 then ya = 255
		else
			exit do
		end if


		SDL_FillRect videobuffer, @rc, 0

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, ya
		SDL_BlitSurface videobuffer2, @rc, videobuffer3, @rc
		SDL_BlitSurface videobuffer, @rc, videobuffer3, @rc
		SDL_BlitSurface videobuffer3, @rc, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if


	loop


	ticks1 = ticks
	ya = 0


	do

 		rc.x = -xofs
		rc.y = 0

		SDL_BlitSurface titleimg, @rcSrc, videobuffer, @rc

 		rc.x = -xofs + 320
		rc.y = 0

		SDL_BlitSurface titleimg, @rcSrc, videobuffer, @rc

 		rc.x = 0
		rc.y = 0

		y! = y! - spd! * fpsr
		for i = 0 to 26
			yy = y! + i * 10
			if yy > -8 and yy < 240 then
				x = 160 - len(story2$(i)) * 4
				sys_print videobuffer, story2$(i), x, yy, 4
			end if

			if yy < 10 and i = 25 then exit do
		next
		'x =
		'sys_print videobuffer, "new game/save/load", x, y, 4


		rc.x = 0
		rc.y = 0
		rc.w = 320
		rc.h = 240

		ya = 255
		if ticks < ticks1 + 1000 then
			ya = 255 * ((ticks - ticks1) / 1000)
			if ya < 0 then ya = 0
			if ya > 255 then ya = 255
		end if

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, ya
		SDL_BlitSurface videobuffer, @rc, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if

		add! = .5 * fpsr
		if add! > 1 then add! = 1
		xofs = xofs + add!
		if xofs >= 320 then xofs = xofs - 320


			SDL_PollEvent(@event)
			keys = SDL_GetKeyState(null)

			if event.type = SDL_KEYDOWN then spd! = 1
			if event.type = SDL_KEYUP then spd! = .2

			if keys[SDLK_ESCAPE] then exit do



	loop


	rc.x = 0
	rc.y = 0
	rc.w = 320
	rc.h = 240

	ticks1 = ticks
	y = 0

	SDL_BlitSurface videobuffer, @rc, videobuffer2, @rc

	do

		if ticks < ticks1 + 1500 then
			y = 255 * ((ticks - ticks1) / 1500)
			if y < 0 then y = 0
			if y > 255 then y = 255
		else
			exit do
		end if

		SDL_FillRect videobuffer, @rc, 0

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, y
		SDL_BlitSurface videobuffer2, @rc, videobuffer3, @rc
		SDL_BlitSurface videobuffer, @rc, videobuffer3, @rc
		SDL_BlitSurface videobuffer3, @rc, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if


	loop


	keywait = 2000 + ticks

	ticks1 = ticks
	y = 0
	do

		SDL_BlitSurface theendimg, @rcSrc, videobuffer, @rc


		y = 255
		if ticks < ticks1 + 1000 then
			y = 255 * ((ticks - ticks1) / 1000)
			if y < 0 then y = 0
			if y > 255 then y = 255
		end if

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, y
		SDL_BlitSurface videobuffer, @rc, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

			SDL_Flip video
			SDL_PumpEvents

			tickspassed = ticks
			ticks = SDL_GetTicks

			tickspassed = ticks - tickspassed
			fpsr = tickspassed / 24

			fp = fp + 1
			if ticks > nextticks then
				nextticks = ticks + 1000
				fps = fp
				fp = 0
			end if

			SDL_PollEvent(@event)
			keys = SDL_GetKeyState(null)

			if event.type = SDL_KEYDOWN and keywait < ticks then exit do

	loop

	SDL_FillRect videobuffer2, @rc, 0
	SDL_FillRect videobuffer3, @rc, 0

	game_theend

end sub


sub game_eventtext (stri$)



	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 320
	rcSrc.h = 240

	SDL_FillRect videobuffer2, @rcSrc, 0
	SDL_FillRect videobuffer3, @rcSrc, 0


	rc.x = 0
	rc.y = 0

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY
	rc2.w = 320
	rc2.h = 240


	x = 160 - 4 * len(stri$)

	ticks = SDL_GetTicks
	pauseticks = ticks + 500
	bticks = ticks


	SDL_BlitSurface videobuffer, null, videobuffer3, null
	SDL_BlitSurface video, @rc2, videobuffer2, null

	do


		SDL_PollEvent(@event)

		keys = SDL_GetKeyState(null)

		if event.type = SDL_KEYDOWN and pauseticks < ticks then exit do

		SDL_BlitSurface videobuffer2, null, videobuffer, null


		fr = 192

		if pauseticks > ticks then fr = 192 * (ticks - bticks) / 500
		if fr > 192 then fr = 192

		SDL_SetAlpha windowimg, 0 OR SDL_SRCALPHA, fr

		SDL_BlitSurface windowimg, null, videobuffer, @rc
		if pauseticks < ticks then sys_print videobuffer, stri$, x, 15, 0


		SDL_SetAlpha windowimg, 0 OR SDL_SRCALPHA, 255

		SDL_BlitSurface videobuffer, null, video, @rc2

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if

		sleep 2
	loop

	SDL_BlitSurface videobuffer3, null, videobuffer, null

	itemticks = ticks + 210


end sub



sub game_handlewalking ()

	dim temp as uinteger ptr, c as uinteger, spd as single, bgc as uinteger
	dim ppx as single, ppy as single, px as single, py as single, opx as single, opy as single
	dim nx as single, ny as single, npx as single, npy as single


	xmax = 20 * 16 - 25
	ymax = 15 * 16 - 25

	px = player.px
	py = player.py
	opx = px
	opy = py

	spd = player.walkspd * fpsr


	nx = (px / 2 + 6)
	ny = (py / 2 + 10)


	npx = px + 12
	npy = py + 20
	lx = (npx - (npx MOD 16)) / 16
	ly = (npy - (npy MOD 16)) / 16

	ramp = rampdata(lx, ly)
	if ramp = 1 and movingup then spd = spd * 2
	if ramp = 1 and movingdown then spd = spd * 2

	if ramp = 2 and movingleft then movingup = 1
	if ramp = 2 and movingright then movingdown = 1

	if ramp = 3 and movingright then movingup = 1
	if ramp = 3 and movingleft then movingdown = 1


	for x = -1 to 1
		for y = -1 to 1
			sx = nx + x
			sy = ny + y

			clipsurround(x + 1, y + 1) = 0
			if sx > -1 and sx < 320 and sy > -1 and sy < 192 then
				temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
				clipsurround(x + 1, y + 1) = *temp
			end if
		next
	next

	if movingup then player.walkdir = 0
	if movingdown then player.walkdir = 1
	if movingleft then player.walkdir = 2
	if movingright then player.walkdir = 3

	if movingup and clipsurround(1, 0) = 0 then
		py = py - spd
		player.walkdir = 0
	elseif movingup and clipsurround(1, 0) > 0 then
		'move upleft
		if movingright = 0 and clipsurround(0, 0) = 0 then
			py = py - spd
			px = px - spd
		end if

		'move upright
		if movingleft = 0 and clipsurround(2, 0) = 0 then
			py = py - spd
			px = px + spd
		end if
	end if
	if movingdown and clipsurround(1, 2) = 0 then
		py = py + spd
		player.walkdir = 1
	elseif movingdown and clipsurround(1, 2) > 0 then
		'move downleft
		if movingright = 0 and clipsurround(0, 2) = 0 then
			py = py + spd
			px = px - spd
		end if

		'move downright
		if movingleft = 0 and clipsurround(2, 2) = 0 then
			py = py + spd
			px = px + spd
		end if
	end if
	if movingleft and clipsurround(0, 1) = 0 then
		px = px - spd
		player.walkdir = 2
	elseif movingleft and clipsurround(0, 1) > 0 then
		'move leftup
		if movingdown = 0 and clipsurround(0, 0) = 0 then
			py = py - spd
			px = px - spd
		end if

		'move leftdown
		if movingup = 0 and clipsurround(0, 2) = 0 then
			py = py + spd
			px = px - spd
		end if
	end if
	if movingright and clipsurround(2, 1) = 0 then
		px = px + spd
		player.walkdir = 3
	elseif movingright and clipsurround(2, 1) > 0 then
		'move rightup
		if movingdown = 0 and clipsurround(2, 0) = 0 then
			px = px + spd
			py = py - spd
		end if

		'move rightdown
		if movingup = 0 and clipsurround(2, 2) = 0 then
			py = py + spd
			px = px + spd
		end if
	end if


	IF px < -8 THEN px = -8
	IF px > xmax THEN px = xmax
	IF py < -8 THEN py = -8
	IF py > ymax THEN py = ymax

	pass = 1

	sx = (px / 2 + 6)
	sy = (py / 2 + 10)
	temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
	bgc = *temp
	if bgc > 0 and bgc <> 1000 then
		px = opx
		py = opy
		pass = 0
	end if


	'push npc
	if pass = 1 then

		FOR i = 1 TO lastnpc

			IF npcinfo(i).hp > 0 THEN

				npx = npcinfo(i).x
				npy = npcinfo(i).y

				opx = npx
				opy = npy

				xdif = player.px - npx
				ydif = player.py - npy

				if player.walkdir = 0 then
					if abs(xdif) <= 8 and ydif > 0 and ydif < 8 then npcinfo(i).y = npcinfo(i).y - spd
				elseif player.walkdir = 1 then
					if abs(xdif) <= 8 and ydif < 0 and ydif > -8 then npcinfo(i).y = npcinfo(i).y + spd
				elseif player.walkdir = 2 then
					if abs(ydif) <= 8 and xdif > 0 and xdif < 8 then npcinfo(i).x = npcinfo(i).x - spd
				elseif player.walkdir = 3 then
					if abs(ydif) <= 8 and xdif < 0 and xdif > -8 then npcinfo(i).x = npcinfo(i).x + spd
				end if


				npx = npcinfo(i).x
				npy = npcinfo(i).y

				sx = int(npx / 2 + 6)
				sy = int(npy / 2 + 10)
				temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
				bgc = *temp

				if bgc > 0 then
					npcinfo(i).x = opx
					npcinfo(i).y = opy
				end if
			end if

		next

	end if

	player.opx = player.px
	player.opy = player.py
	player.px = px
	player.py = py

	IF player.px <> player.opx OR player.py <> player.opy THEN player.walkframe = player.walkframe + animspd * fpsr
	if player.walkframe >= 16 then player.walkframe = player.walkframe - 16



	'walking over items to pickup :::


	o = objmap (lx, ly)

	if o > -1 then
		'fsk
		if objectinfo(o, 4) = 2 and inventory(0) < 9 then
			objmap (lx, ly) = -1

			inventory(0) = inventory(0) + 1
			if inventory(0) > 9 then inventory(0) = 9
			game_addFloatIcon 6, lx * 16, ly * 16

			objmapf (curmap, lx, ly) = 1

			if menabled = 1 and opeffects = 1 then
				snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
				FSOUND_setVolume(snd, opeffectsvol)
			end if

		end if
		if objectinfo(o, 5) = 7 and inventory(1) < 9 then
			objmap (lx, ly) = -1

			inventory(1) = inventory(1) + 1
			if inventory(1) > 9 then inventory(1) = 9
			game_addFloatIcon 12, lx * 16, ly * 16

			objmapf (curmap, lx, ly) = 1

			if menabled = 1 and opeffects = 1 then
				snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
				FSOUND_setVolume(snd, opeffectsvol)
			end if

		end if
		if objectinfo(o, 5) = 9 and inventory(2) < 9 and (curmap = 41 and scriptflag(9, 1) = 0) then
			objmap (lx, ly) = -1

			inventory(2) = inventory(2) + 1
			if inventory(2) > 9 then inventory(2) = 9
			game_addFloatIcon 17, lx * 16, ly * 16

			objmapf (curmap, lx, ly) = 1
			if curmap = 41 then scriptflag(9, 1) = 1

			if menabled = 1 and opeffects = 1 then
				snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
				FSOUND_setVolume(snd, opeffectsvol)
			end if

	end if

	if objectinfo(o, 5) = 9 and inventory(2) < 9 then
			objmap (lx, ly) = -1

			inventory(2) = inventory(2) + 1
			if inventory(2) > 9 then inventory(2) = 9
			game_addFloatIcon 17, lx * 16, ly * 16

			objmapf (curmap, lx, ly) = 1

			if menabled = 1 and opeffects = 1 then
				snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
				FSOUND_setVolume(snd, opeffectsvol)
			end if

		end if
	end if



end sub

sub game_loadmap (mapnum)

	dim ccc as uinteger

	ccc = SDL_MapRGB(clipbg->format, 255,255,255)

	curmap = mapnum


	dim trect as SDL_Rect
	trect.x = 0
	trect.y	= 0
	trect.w = 320
	trect.h = 240
	SDL_FillRect mapbg, @trect, 0
	tt = SDL_FillRect (clipbg, @trect, ccc)
	tt = SDL_FillRect (clipbg2, @trect, ccc)

	forcepause = 0
	cloudson = 0
	if mapnum < 6 then cloudson = 1
	if mapnum > 41 then cloudson = 1
	if mapnum > 47 then cloudson = 0
	if mapnum = 52 then cloudson = 1
	if mapnum = 60 then cloudson = 1
	if mapnum = 50 then cloudson = 1
	if mapnum = 54 then cloudson = 1
	if mapnum = 58 then cloudson = 1
	if mapnum = 62 then cloudson = 1
	if mapnum = 83 then cloudson = 1


	'-----------special case
	dontdrawover = 0
	if mapnum = 24 then dontdrawover = 1

	if (mapnum = 53 or mapnum = 57 or mapnum = 61 or mapnum = 65 or mapnum = 62) and scriptflag(60,0) > 0 then mapnum = mapnum + 100
	if (mapnum = 161 or mapnum = 162) and scriptflag(60,0) = 2 then mapnum = mapnum + 100


	for i = 0 to maxspell

		spellinfo(i).frame = 0

	next

	roomlock = 0


	t$ = LTRIM$(RTRIM$(STR$(mapnum)))

	FOR i = 0 TO 3
		IF LEN(t$) < 4 THEN t$ = "0" + t$
	NEXT

	dim tempsurf as SDL_Surface ptr

	tempsurf = IMG_Load ("mapdb\" + t$ + ".pcx")


	dim tempmap(320, 200)
	open "mapdb\" + t$ + ".map" for input as #1

	for x = 0 to 319
		for y = 0 to 199

			input #1, tempmap(x, y)

		next
	next

	close #1

	for x = 0 to 319
		for y = 0 to 239
			triggerloc(x,y) = -1
		next
	next


	OPEN "mapdb\" + t$ + ".trg" FOR INPUT AS #1

		INPUT #1, ntriggers
		FOR i = 0 TO ntriggers - 1

			input #1, mapx
			input #1, mapy
			input #1, trig

			triggerloc(mapx, mapy) = trig

		NEXT

	CLOSE #1


	FOR y = 0 TO 23
		FOR x = 0 TO 39
			rampdata(x, y) = tempmap(3 * 40 + x, y + 40)
		next
	next

	FOR y = 0 TO 23
		FOR x = 0 TO 39
			FOR l = 0 TO 2
				for a = 0 to 2
					tileinfo (l, x, y, a) = 0
				next
			next
		next
	next

	if scriptflag(4, 0) = 1 and curmap = 4 then
		triggerloc(9,7) = 5004
		tempmap(9,7) = 41
		tempmap(9,7 + 40) = 0
	end if

	FOR y = 0 TO 23
		FOR x = 0 TO 39
			FOR l = 0 TO 2

				ly = y
				lx = x + l * 40

				'''tile
				curtile = tempmap(lx, ly)
				curtilelayer = tempmap(lx, ly + 40)


					IF curtile > 0 THEN
					curtile = curtile - 1
					curtilel = curtilelayer
					curtilex = curtile MOD 20
					curtiley = (curtile - curtilex) / 20

					tileinfo (l, x, y, 0) = curtile + 1
					tileinfo (l, x, y, 1) = curtilelayer

					rcSrc.x = curtilex * 16
					rcSrc.y = curtiley * 16
					rcSrc.w = 16
					rcSrc.h = 16

					rcDest.x = x * 16
					rcDest.y = y * 16
					rcDest.w = 16
					rcDest.h = 16

					pass = 0

					if l = 2 and curtilel = 1 then
						for ff = 0 to 5
							ffa = 20 * 5 - 1 + ff * 20
							ffb = 20 * 5 + 4 + ff * 20
							if curtile > ffa and curtile < ffb then
								SDL_SetAlpha tiles(curtilel), 0 OR SDL_SRCALPHA, 128
								pass = 1
							end if
						next
					end if
					if l = 1 and curtilel = 2 then
						for ff = 0 to 4
							ffa = 20 * (5 + ff) + 3
							if curtile = ffa then
								SDL_SetAlpha tiles(curtilel), 0 OR SDL_SRCALPHA, 192
								pass = 1
							end if
						next
					end if


					SDL_BlitSurface tiles(curtilel), @rcSrc, mapbg, @rcDest

					SDL_SetAlpha tiles(curtilel), 0 OR SDL_SRCALPHA, 255

					rcDest.x = x * 8
					rcDest.y = y * 8
					rcDest.w = 8
					rcDest.h = 8

					tt = SDL_FillRect (clipbg, @rcDest, 0)
					END IF

			NEXT
		NEXT
	NEXT


	FOR x = 0 TO 39
		FOR y = 0 TO 23

			d = tempmap(3 * 40 + x, y)

			clip = 0
			npc = 0
			obj = 0

			if scriptflag(4, 0) = 1 and x = 9 and y = 7 then d = 99

			IF d > 0 THEN
				clip = d MOD 2
				d = (d - clip) / 2
				npc = d MOD 2
				d = (d - npc) / 2
				obj = d MOD 2

				if d = 99 and x = 9 and y = 7 then clip = 1

				IF clip THEN

					if d <> 99 then d = tempmap(6 * 40 + x, y)
					if d = 99 then d = 1

					x1 = x * 8
					y1 = y * 8

					IF d = 1 THEN
						FOR i = 0 TO 7
							sys_line clipbg, x1, y1 + i, x1 + 7 - i, y1 + i, ccc
						NEXT
					ELSEIF d = 2 THEN
						sys_line clipbg, x1, y1, x1 + 7, y1, ccc
						sys_line clipbg, x1, y1 + 1, x1 + 7, y1 + 1, ccc
					ELSEIF d = 3 THEN
						FOR i = 0 TO 7
							sys_line clipbg, x1 + i, y1 + i, x1 + 7, y1 + i, ccc
						NEXT
					ELSEIF d = 4 THEN
						sys_line clipbg, x1, y1, x1, y1 + 7, ccc
						sys_line clipbg, x1 + 1, y1, x1 + 1, y1 + 7, ccc
					ELSEIF d = 5 THEN
						rcDest.x = x1
						rcDest.y = y1
						rcDest.w = 8
						rcDest.h = 8
						t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))
					ELSEIF d = 6 THEN
						sys_line clipbg, x1 + 7, y1, x1 + 7, y1 + 7, ccc
						sys_line clipbg, x1 + 6, y1, x1 + 6, y1 + 7, ccc
					ELSEIF d = 7 THEN
						FOR i = 0 TO 7
							sys_line clipbg, x1, y1 + i, x1 + i, y1 + i, ccc
						NEXT
					ELSEIF d = 8 THEN
						sys_line clipbg, x1, y1 + 7, x1 + 7, y1 + 7, ccc
						sys_line clipbg, x1, y1 + 7, x1 + 6, y1 + 6, ccc
					ELSEIF d = 9 THEN
						FOR i = 0 TO 7
							sys_line clipbg, x1 + 7 - i, y1 + i, x1 + 7, y1 + i, ccc
						NEXT
					END IF
				END IF

			END IF

		NEXT
	NEXT



	lastobj = 0
	lastnpc = 0

	for i = 0 to maxnpc
		npcinfo(i).onmap = 0
	next


	FOR x = 0 TO 19
		FOR y = 0 TO 19

			d = tempmap(3 * 40 + x, y)

			clip = 0
			npc = 0
			obj = 0
			IF d > 0 THEN
				clip = d MOD 2
				d = (d - clip) / 2
				npc = d MOD 2
				d = (d - npc) / 2
				obj = d MOD 2
			END IF

			objmap (x, y) = -1

			IF obj = 1 THEN

				o = tempmap(5 * 40 + x, y)

				if objmapf (curmap, x, y) = 0 then

					objmap (x, y) = o

					IF objectinfo(o, 0) > 1 THEN
						IF o > lastobj THEN lastobj = o
					END IF

					x1 = x * 8
					y1 = y * 8

					rcDest.x = x1
					rcDest.y = y1
					rcDest.w = 8
					rcDest.h = 8

					if objectinfo(o, 4) = 1 then t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))
					if objectinfo(o, 4) = 3 then t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))
				end if


			END IF
			IF npc = 1 THEN

				o = tempmap(4 * 40 + x, y)

				IF o > lastnpc THEN lastnpc = o

				npcinfo(o).x = x * 16 - 4
				npcinfo(o).y = y * 16 - 5

				npcinfo(o).walkdir = 1
				npcinfo(o).onmap = 1
			END IF

		NEXT
	NEXT


	if curmap = 62 and scriptflag(8, 0) > 0 then lastnpc = 0
	if curmap = 73 and scriptflag(12, 0) > 0 then lastnpc = 0
	if curmap = 81 and scriptflag(13, 0) > 0 then lastnpc = 0

	if curmap = 73 and scriptflag(12, 0) = 0 then roomlock = 1
	if curmap = 81 and scriptflag(13, 0) = 0 then roomlock = 1
	if curmap = 83 and scriptflag(15, 0) = 0 then roomlock = 1
	if curmap = 82 then roomlock = 1

	open "mapdb\" + t$ + ".npc" FOR INPUT AS #1

		FOR i = 0 TO maxnpc
			input #1, npcinfo(i).spriteset
			input #1, npcinfo(i).x1
			input #1, npcinfo(i).y1
			input #1, npcinfo(i).x2
			input #1, npcinfo(i).y2
			input #1, npcinfo(i).movementmode
			input #1, npcinfo(i).hp
			input #1, npcinfo(i).item1
			input #1, npcinfo(i).item2
			input #1, npcinfo(i).item3
			input #1, npcinfo(i).script

			'baby dragon
			if npcinfo(i).spriteset = 1 then
				npcinfo(i).hp = 12
				npcinfo(i).attackdelay = 2000

				npcinfo(i).attackdamage = 2
				npcinfo(i).spelldamage = 0

				npcinfo(i).walkspd = 1

				if int(rnd * 5) = 0 then npcinfo(i).hp = 0

			end if

			'onewing
			if npcinfo(i).spriteset = 2 then
				npcinfo(i).hp = 200
				npcinfo(i).attackdelay = 2000
				npcinfo(i).swayspd = 1

				npcinfo(i).attackdamage = 24
				npcinfo(i).spelldamage = 30

				npcinfo(i).walkspd = 1.4
				npcinfo(i).castpause = ticks

			end if


			'boss1
			if npcinfo(i).spriteset = 3 then
				npcinfo(i).hp = 300
				npcinfo(i).attackdelay = 2200

				npcinfo(i).attackdamage = 0
				npcinfo(i).spelldamage = 30

				npcinfo(i).walkspd = 1.2

			end if


			'black knights
			if npcinfo(i).spriteset = 4 then
				npcinfo(i).hp = 200
				npcinfo(i).attackdelay = 2800

				npcinfo(i).attackdamage = 0
				npcinfo(i).spelldamage = 30

				npcinfo(i).walkspd = 1

			end if


			'boss2 firehydra
			if npcinfo(i).spriteset = 5 then
				npcinfo(i).hp = 600
				npcinfo(i).attackdelay = 2200

				npcinfo(i).attackdamage = 50
				npcinfo(i).spelldamage = 30

				npcinfo(i).walkspd = 1.3

				npcinfo(i).swayangle = 0
			end if

			'baby fire dragon
			if npcinfo(i).spriteset = 6 then
				npcinfo(i).hp = 20
				npcinfo(i).attackdelay = 1500

				npcinfo(i).attackdamage = 0
				npcinfo(i).spelldamage = 12

				npcinfo(i).walkspd = 1

				if int(rnd * 5) = 0 then npcinfo(i).hp = 0

			end if

			'priest1
			if npcinfo(i).spriteset = 7 then
				npcinfo(i).hp = 40
				npcinfo(i).attackdelay = 5000

				npcinfo(i).attackdamage = 0
				npcinfo(i).spelldamage = 8

				npcinfo(i).walkspd = 1

				if int(rnd * 8) = 0 then npcinfo(i).hp = 0

			end if

			'yellow fire dragon
			if npcinfo(i).spriteset = 8 then
				npcinfo(i).hp = 100
				npcinfo(i).attackdelay = 1500

				npcinfo(i).attackdamage = 0
				npcinfo(i).spelldamage = 24

				npcinfo(i).walkspd = 1

				if int(rnd * 5) = 0 then npcinfo(i).hp = 0

			end if


			'twowing
			if npcinfo(i).spriteset = 9 then
				npcinfo(i).hp = 140
				npcinfo(i).attackdelay = 2000
				npcinfo(i).swayspd = 1

				npcinfo(i).attackdamage = 30
				npcinfo(i).spelldamage = 0

				npcinfo(i).walkspd = 1

				npcinfo(i).castpause = 0
			end if


			'dragon2
			if npcinfo(i).spriteset = 10 then
				npcinfo(i).hp = 80
				npcinfo(i).attackdelay = 1500

				npcinfo(i).attackdamage = 24
				npcinfo(i).spelldamage = 0

				npcinfo(i).walkspd = 1

				npcinfo(i).floating = int(rnd * 16)

			end if

			'end boss
			if npcinfo(i).spriteset = 11 then
				npcinfo(i).hp = 1200
				npcinfo(i).attackdelay = 2000

				npcinfo(i).attackdamage = 100
				npcinfo(i).spelldamage = 60

				npcinfo(i).walkspd = 1

				npcinfo(i).floating = int(rnd * 16)
			end if

			'bat kitty
			if npcinfo(i).spriteset = 12 then
				npcinfo(i).hp = 800
				npcinfo(i).attackdelay = 2000

				npcinfo(i).attackdamage = 100
				npcinfo(i).spelldamage = 50

				npcinfo(i).walkspd = 1

				npcinfo(i).floating = int(rnd * 16)
			end if

			if npcinfo(i).onmap = 0 then npcinfo(i).hp = 0

			npcinfo(i).maxhp = npcinfo(i).hp

			npcinfo(i).attacking = 0
			npcinfo(i).attackframe = 0
			npcinfo(i).cattackframe = 0
			npcinfo(i).attackspd = 1.5
			npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay * (1 + rnd * 2)


			if npcinfo(i).spriteset = 2 or npcinfo(i).spriteset = 9 then
				npcinfo(i).bodysection(0).sprite = 0
				npcinfo(i).bodysection(1).sprite = 1
				npcinfo(i).bodysection(2).sprite = 2
				npcinfo(i).bodysection(3).sprite = 3
				npcinfo(i).bodysection(4).sprite = 4
				npcinfo(i).bodysection(5).sprite = 3
				npcinfo(i).bodysection(6).sprite = 3
				npcinfo(i).bodysection(7).sprite = 5

				npcinfo(i).bodysection(0).bonelength = 8
				npcinfo(i).bodysection(1).bonelength = 7
				npcinfo(i).bodysection(2).bonelength = 6
				npcinfo(i).bodysection(3).bonelength = 4
				npcinfo(i).bodysection(4).bonelength = 4
				npcinfo(i).bodysection(5).bonelength = 4
				npcinfo(i).bodysection(6).bonelength = 4

				for f = 0 to 7
					npcinfo(i).bodysection(f).x = npcinfo(i).x + 12
					npcinfo(i).bodysection(f).y = npcinfo(i).y + 14
				next

				npcinfo(i).headtargetx(0) = npcinfo(i).x + 12
				npcinfo(i).headtargety(0) = npcinfo(i).y + 14

			end if

			if npcinfo(i).spriteset = 5 then
				for f = 0 to 29
					npcinfo(i).bodysection(f).x = npcinfo(i).x + 12
					npcinfo(i).bodysection(f).y = npcinfo(i).y + 14
				next

				for f = 0 to 2
					npcinfo(i).headtargetx(f) = npcinfo(i).x + 12
					npcinfo(i).headtargety(f) = npcinfo(i).y + 14

					npcinfo(i).attacking2(f) = 0
					npcinfo(i).attackframe2(f) = 0
				next

			end if


			if npcinfo(i).script = 2 then
				roomlock = 1
				if scriptflag(2, 0) > 0 then
					roomlock = 0
					npcinfo(i).hp = 0
				end if
			end if

			if npcinfo(i).script = 3 then
				roomlock = 1
				if scriptflag(3, 0) > 0 then
					roomlock = 0
					npcinfo(i).hp = 0
				end if
			end if


			if npcinfo(i).script = 5 then
				roomlock = 1
				if scriptflag(5, 0) > 0 then
					roomlock = 0
					npcinfo(i).hp = 0
				end if
			end if

			if npcinfo(i).script = 15 then
				roomlock = 1
				if scriptflag(15, 0) > 0 then
					roomlock = 0
					npcinfo(i).hp = 0
				end if
			end if

			npcinfo(i).pause = ticks

		NEXT

	CLOSE #1


	'academy master key
	if curmap = 34 and scriptflag (2, 0) = 1 then
			cx = 9
			cy = 7

			objmap(cx, cy) = 5

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

	end if

	'academy crystal
	if curmap = 24 and player.foundcrystal = 0 and scriptflag(3, 0) = 1 then
			cx = 9
			cy = 7

			objmap(cx, cy) = 6

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

	end if

	'gardens master key
	if curmap = 62 and scriptflag (8, 0) = 1 then
			cx = 13
			cy = 7

			objmap(cx, cy) = 5

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

	end if

	'gardens fidelis sword
	if curmap = 66 and scriptflag (5, 0) = 1 and player.sword = 1 then
			cx = 9
			cy = 6

			objmap(cx, cy) = 9

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

	end if


	'citadel armour
	if curmap = 73 and scriptflag (12, 0) = 1 and player.armour = 1 then
			cx = 8
			cy = 7

			objmap(cx, cy) = 16

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

	end if

	'citadel master key
	if curmap = 81 and scriptflag (13, 0) = 1 then
			cx = 11
			cy = 10

			objmap(cx, cy) = 5

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

	end if


	'max ups
		if curmap = 83 and scriptflag (15, 0) = 1 and player.sword < 3 then
			cx = 6
			cy = 8

			objmap(cx, cy) = 18

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

		end if

		if curmap = 83 and scriptflag (16, 0) = 1 and player.shield < 3 then

			cx = 9
			cy = 8

			objmap(cx, cy) = 19

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))

		end if

		if curmap = 83 and scriptflag (17, 0) = 1 and player.armour < 3 then
			cx = 12
			cy = 8

			objmap(cx, cy) = 20

			rcDest.x = cx * 8
			rcDest.y = cy * 8
			rcDest.w = 8
			rcDest.h = 8

			npx = player.px + 12
			npy = player.py + 20

			lx = (npx - (npx MOD 16)) / 16
			ly = (npy - (npy MOD 16)) / 16

			if lx = cx and ly = cy then player.py = player.py + 16

			t = SDL_FillRect(clipbg, @rcDest, SDL_MapRGB(clipbg->format, 255,255,255))
		end if


	SDL_BlitSurface clipbg, null, clipbg2, null

	SDL_FreeSurface tempsurf


end sub


sub game_main ()

	game_title 0
	game_saveloadnew


	SDL_Quit
	FSOUND_StopSound (FSOUND_ALL)
	end 1
end sub


sub game_newgame ()


dim xofs as single


	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 320
	rcSrc.h = 240

	SDL_FillRect videobuffer2, @rcSrc, 0
	SDL_FillRect videobuffer3, @rcSrc, 0


	rc.x = 0
	rc.y = 0

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY
	rc2.w = 320
	rc2.h = 240


	x = 160 - 4 * len(stri$)


	pauseticks = ticks + 500
	bticks = ticks


	SDL_BlitSurface videobuffer, null, videobuffer3, null
	SDL_BlitSurface video, @rc2, videobuffer2, null

	cursel = 0

	keypause = ticks + 220

	spd! = .2
	y! = 140

	if menabled = 1 and opmusic = 1 then
		FSOUND_stopSound(FSOUND_ALL)
		musicchannel = FSOUND_playSound(FSOUND_FREE, mendofgame)
		FSOUND_setVolume(musicchannel, 0)
	end if

	secsingame = 0
	secstart = 0
	do

		ld! = ld! + 4 * fpsr
		if ld! > opmusicvol then ld! = opmusicvol
		if menabled = 1 and ldstop = 0 then
			FSOUND_setVolume(musicchannel, ld!)
			if ld! = opmusicvol then ldstop = 1
		end if

 		rc.x = -xofs
		rc.y = 0

		SDL_BlitSurface titleimg, null, videobuffer, @rc

 		rc.x = -xofs + 320
		rc.y = 0

		SDL_BlitSurface titleimg, null, videobuffer, @rc

 		rc.x = 0
		rc.y = 0

		y! = y! - spd! * fpsr
		for i = 0 to 37
			yy = y! + i * 10
			if yy > -8 and yy < 240 then
				x = 160 - len(story$(i)) * 4
				sys_print videobuffer, story$(i), x, yy, 4
			end if

			if yy < 10 and i = 37 then exit do
		next
		'x =
		'sys_print videobuffer, "new game/save/load", x, y, 4


		rc.x = 0
		rc.y = 0
		rc.w = 320
		rc.h = 240

		SDL_BlitSurface videobuffer, @rc, video, @rc2

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if

		add! = .5 * fpsr
		if add! > 1 then add! = 1
		xofs = xofs + add!
		if xofs >= 320 then xofs = xofs - 320


			SDL_PollEvent(@event)
			keys = SDL_GetKeyState(null)

			if event.type = SDL_KEYDOWN then spd! = 1
			if event.type = SDL_KEYUP then spd! = .2

			if keys[SDLK_ESCAPE] then exit do



		sleep 1
	loop



	player.px = 0
	player.py = 0
	player.opx = 0
	player.opy = 0
	player.walkdir = 0
	player.walkframe = 0
	player.walkspd = 0
	player.attackframe = 0
	player.attackspd = 0
	player.hp = 0
	player.maxhp = 0
	player.hpflash = 0
	player.level = 0
	player.sword = 0
	player.shield = 0
	player.armour = 0
	for i = 0 to 5
		player.foundspell(i) = 0
		player.spellcharge(i) = 0
	next
	player.flasks = 0
	player.foundcrystal = 0
	player.crystalcharge = 0
	player.attackstrength = 0
	player.spelldamage = 0
	player.sworddamage = 0
	player.masterkey = 0
	player.exp = 0
	player.nextlevel = 0


	for a = 0 to 99
		for b = 0 to 9
			scriptflag (a, b) = 0
		next
	next

	for a = 0 to 999
		for b = 0 to 20
			for c = 0 to 14
				objmapf(a, b, c) = 0
			next
		next
	next

	for a = 0 to 4
		inventory(a) = 0
	next

	for a = 0 to 200
		roomlocks(a) = 0
	next

	roomlocks(66) = 2
	roomlocks(24) = 2
	roomlocks(17) = 1
	roomlocks(34) = 1
	roomlocks(50) = 1
	roomlocks(73) = 1
	roomlocks(82) = 2

	player.walkspd = 1.1
	animspd = .5
	attacking = 0
	player.attackspd = 1.5

	player.sword = 1
	player.level = 1
	player.nextlevel = 50
	player.shield = 1
	player.armour = 1
	player.hp = 14
	player.maxhp = player.hp

	player.sworddamage = player.level * 2
	player.spelldamage = player.level * 1.5

	player.px = 15*16 - 4
	player.py = 6*16 - 4
	player.walkdir = 1

	pgardens = 0
	ptown = 0
	pboss = 0
	pacademy = 0
	pcitadel = 0

	game_loadmap 2

	game_playgame
end sub


sub game_playgame ()

	game_swash

	if pmenu = 1 and menabled = 1 then
		FSOUND_stopSound(menuchannel)
		pmenu = 0
	end if

	do
		if forcepause = 0 then
			game_updanims
			game_updnpcs
		end if

		game_checktrigger
		game_checkinputs

		if forcepause = 0 then game_handlewalking

		game_updatey
		game_drawview

		game_updmusic

		sys_update

	loop

end sub


sub game_processtrigger (trignum)

	npx = player.px + 12
	npy = player.py + 20

	lx = (npx - (npx MOD 16)) / 16
	ly = (npy - (npy MOD 16)) / 16


	trigtype = triggers(trignum, 0)

	if roomlock = 1 then exit sub
	'map jump------------------------------
	if trigtype = 0 then

		tx = triggers(trignum, 1)
		ty = triggers(trignum, 2)
		tmap = triggers(trignum, 3)
		tjumpstyle = triggers(trignum, 4)

		if roomlocks(tmap) > 0 then
			if saidlocked = 0 then game_eventtext "Locked"
			saidlocked = 1
			canusekey = 1
			locktype = roomlocks(tmap)
			roomtounlock = tmap
			exit sub
		end if

		if tmap = 1 then
			if saidjammed = 0 then game_eventtext "Door Jammed!"
			saidjammed = 1
			exit sub
		end if


		saidlocked = 0
		saidjammed = 0

		'loc-sxy+oldmaploc
		if tjumpstyle = 0 then

			tsx = triggers(trignum, 5)
			tsy = triggers(trignum, 6)

			player.px = (tx - tsx) * 16 + player.px
			player.py = (ty - tsy) * 16 + player.py

			if tmap > 0 then

				if menabled = 1 and opeffects = 1 then
					snd = FSOUND_PlaySound(FSOUND_FREE, sfx(snddoor))
					FSOUND_setVolume(snd, opeffectsvol)
				end if

				game_loadmap tmap
				game_swash
			end if
		end if
	end if

	for i = 0 to maxfloat

		floattext(i, 0) = 0
		floaticon(i, 0) = 0
	next



end sub

sub game_saveloadnew ()

	dim y as single, yy as single

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY



	SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255
	SDL_SetAlpha saveloadimg, 0 OR SDL_SRCALPHA, 192


	currow = 0
	curcol = 0

	lowerlock = 0

	tickpause = ticks + 150

	ticks = SDL_GetTicks
	ticks1 = ticks
	do


		SDL_FillRect videobuffer, null, 0

		y = y + 1 * fpsr

		rcDest.x = 256 + 256 * cos(3.141592 / 180 * clouddeg * 40)
		rcDest.y = 192 + 192 * sin(3.141592 / 180 * clouddeg * 40)
		rcDest.w = 320
		rcDest.h = 240

		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 128
		SDL_BlitSurface cloudimg, @rcDest, videobuffer, null
		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 64


		rcDest.x = 256 '+ 256 * cos(3.141592 / 180 * clouddeg)
		rcDest.y = 192 '+ 192 * sin(3.141592 / 180 * clouddeg)
		rcDest.w = 320
		rcDest.h = 240

		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 128
		SDL_BlitSurface cloudimg, @rcDest, videobuffer, null
		SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 64


		SDL_BlitSurface saveloadimg, null, videobuffer, null


		if tickpause < ticks then

			SDL_PollEvent(@event)
			if event.type = SDL_KEYDOWN then
			keys = SDL_GetKeyState(null)

			itemticks = ticks + 220

			select case event.key.keysym.sym
				case SDLK_ESCAPE
					if lowerlock = 0 then exit sub
					lowerlock = 0
					currow = 0
					tickpause = ticks + 125
			end select

				if keys[SDLK_RETURN] or keys[SDLK_SPACE] then
						if currow = 0 and curcol = 4 then
							SDL_Quit
							FSOUND_StopSound (FSOUND_ALL)
							end 1
						end if
						if currow = 0 and curcol = 3 then exit sub
						if currow = 0 and curcol = 0 then game_newgame

						if currow = 0 and curcol = 1 then
							lowerlock = 1
							currow = 1 + saveslot
							tickpause = ticks + 125
						end if
						if currow = 0 and curcol = 2 then
							lowerlock = 1
							currow = 1
							tickpause = ticks + 125
						end if


						if lowerlock = 1 and curcol = 1 and tickpause < ticks then


							open "data\player" + ltrim$(rtrim$(str$(currow - 1))) + ".sav" for output as #1


								print #1, player.level

								print #1, (secstart + secsingame)
								print #1, "a"

								print #1, player.px
								print #1, player.py
								print #1, player.opx
								print #1, player.opy
								print #1, player.walkdir
								print #1, player.walkframe
								print #1, player.walkspd
								print #1, player.attackframe
								print #1, player.attackspd
								print #1, player.hp
								print #1, player.maxhp
								print #1, player.hpflash
								print #1, player.level
								print #1, player.sword
								print #1, player.shield
								print #1, player.armour
								for i = 0 to 5
									print #1, player.foundspell(i)
									print #1, player.spellcharge(i)
								next
								for a = 0 to 4
									print #1, inventory(a)
								next
								print #1, player.foundcrystal
								print #1, player.crystalcharge
								print #1, player.attackstrength
								print #1, player.spelldamage
								print #1, player.sworddamage
								print #1, player.masterkey
								print #1, player.exp
								print #1, player.nextlevel
								for a = 0 to 99
									for b = 0 to 9
										print #1, scriptflag (a,b)
									next
								next
								print #1, curmap

								for a = 0 to 999
									for b = 0 to 20
										for c = 0 to 14
											print #1, objmapf(a, b, c)
										next
									next
								next

								for a = 0 to 200
									print #1, roomlocks(a)
								next

								print #1, player.spellstrength

							close #1

							secstart = secstart + secsingame
							secsingame = 0
							lowerlock = 0
							saveslot = currow - 1
							currow = 0
						end if

						if lowerlock = 1 and curcol = 2 and tickpause < ticks then


							open "data\player" + ltrim$(rtrim$(str$(currow - 1))) + ".sav" for input as #1


								input #1, player.level

								input #1, secstart
								input #1, aa$


								input #1, player.px
								input #1, player.py
								input #1, player.opx
								input #1, player.opy
								input #1, player.walkdir
								input #1, player.walkframe
								input #1, player.walkspd
								input #1, player.attackframe
								input #1, player.attackspd
								input #1, player.hp
								input #1, player.maxhp
								input #1, player.hpflash
								input #1, player.level
								input #1, player.sword
								input #1, player.shield
								input #1, player.armour
								for i = 0 to 5
									input #1, player.foundspell(i)
									input #1, player.spellcharge(i)
								next
								for a = 0 to 4
									input #1, inventory(a)
								next
								input #1, player.foundcrystal
								input #1, player.crystalcharge
								input #1, player.attackstrength
								input #1, player.spelldamage
								input #1, player.sworddamage
								input #1, player.masterkey
								input #1, player.exp
								input #1, player.nextlevel
								for a = 0 to 99
									for b = 0 to 9
										input #1, scriptflag (a,b)
									next
								next
								input #1, curmap

								for a = 0 to 999
									for b = 0 to 20
										for c = 0 to 14
											input #1, objmapf(a, b, c)
										next
									next
								next

								for a = 0 to 200
									input #1, roomlocks(a)
								next

								input #1, player.spellstrength

							player.walkspd = 1.1
							animspd = .5
							attacking = 0
							Player.attackspd = 1.5

							pgardens = 0
							ptown = 0
							pboss = 0
							pacademy = 0
							pcitadel = 0


							close #1

							FSOUND_stopSound(FSOUND_ALL)

							secsingame = 0
							saveslot = currow - 1
							cmap = curmap
							game_loadmap cmap
							game_playgame

					end if
					tickpause = ticks + 125

				end if

			select case event.key.keysym.sym
				case SDLK_DOWN
					if lowerlock = 1 then
						currow = currow + 1
						if currow = 5 then currow = 1
						tickpause = ticks + 125
					end if

				case SDLK_UP
					if lowerlock = 1 then
						currow = currow - 1
						if currow = 0 then currow = 4
						tickpause = ticks + 125
					end if

				case SDLK_LEFT
					if lowerlock = 0 then
						curcol = curcol - 1
						if curcol = -1 then curcol = 3
						tickpause = ticks + 125
					end if

				case SDLK_RIGHT
					if lowerlock = 0 then
						curcol = curcol + 1
						if curcol = 4 then curcol = 0
						tickpause = ticks + 125
					end if


			end select
			end if
		end if


		'savestats---------------------------------

		for ff = 0 to 3

			open "data\player" + ltrim$(rtrim$(str$(ff))) + ".sav" for input as #1
				input #1, plevel
				if plevel > 0 then
					playera.level = plevel

					input #1, asecstart
					input #1, aa$

					input #1, playera.px
					input #1, playera.py
					input #1, playera.opx
					input #1, playera.opy
					input #1, playera.walkdir
					input #1, playera.walkframe
					input #1, playera.walkspd
					input #1, playera.attackframe
					input #1, playera.attackspd
					input #1, playera.hp
					input #1, playera.maxhp
					input #1, playera.hpflash
					input #1, playera.level
					input #1, playera.sword
					input #1, playera.shield
					input #1, playera.armour
					for i = 0 to 5
						input #1, playera.foundspell(i)
						input #1, playera.spellcharge(i)
					next
					for i = 0 to 4
						input #1, a
					next
					input #1, playera.foundcrystal
					input #1, playera.crystalcharge
					input #1, playera.attackstrength
					input #1, playera.spelldamage
					input #1, playera.sworddamage
					input #1, playera.masterkey
					input #1, playera.exp
					input #1, playera.nextlevel

				end if
			close #1

			if plevel > 0 then

				sx = 8
				sy = 57 + ff * 48

				ase = asecstart
				h = ((ase - (ase mod 3600)) / 3600)
				ase = (ase - h * 3600)
				m = ((ase - (ase mod 60)) / 60)
				s = (ase - m * 60)

				h$ = ltrim$(rtrim$(str$(h)))
				if len(h$) = 1 then h$ = "0" + h$
				m$ = ltrim$(rtrim$(str$(m)))
				if len(m$) = 1 then m$ = "0" + m$
				s$ = ltrim$(rtrim$(str$(s)))
				if len(s$) = 1 then s$ = "0" + s$

				t$ = "Game Time: " + h$ + ":" + m$ + ":" + s$
				sys_print videobuffer, t$, 160 - len(t$) * 4, sy, 0


				sy = sy + 11

				sx  = 12
				cc = 0

				sys_print videobuffer, "Health: "+ltrim$(rtrim$(str$(playera.hp))) + "/" + ltrim$(rtrim$(str$(playera.maxhp))), sx, sy, cc

				f$ = ltrim$(rtrim$(str$(playera.level)))
				if playera.level = 22 then f$ = "MAX"
				sys_print videobuffer, "Level : "+f$, sx, sy + 11, 0


				rcSrc.x = sx + 15 * 8 + 24
				rcSrc.y = sy + 1

				ss = (playera.sword - 1) * 3
				if playera.sword = 3 then ss = 18
				SDL_BlitSurface itemimg(ss), null, videobuffer, @rcSrc

				rcSrc.x = rcSrc.x + 16
				ss = (playera.shield - 1) * 3 + 1
				if playera.shield = 3 then ss = 19
				SDL_BlitSurface itemimg(ss), null, videobuffer, @rcSrc

				rcSrc.x = rcSrc.x + 16
				ss = (playera.armour - 1) * 3 + 2
				if playera.armour = 3 then ss = 20
				SDL_BlitSurface itemimg(ss), null, videobuffer, @rcSrc

				nx = rcSrc.x + 13 + 3*8
				rcSrc.x = nx - 17

				if playera.foundcrystal = 1 then
					rcSrc.x = rcSrc.x + 17

					SDL_BlitSurface itemimg(7), null, videobuffer, @rcSrc

					ccc = SDL_MapRGB(videobuffer->format, 0, 32, 32)

					for i = 0 to 4
						rcSrc.x = rcSrc.x + 17

						if playera.foundspell(i) = 1 then SDL_BlitSurface itemimg(8 + i), null, videobuffer, @rcSrc

					next
				end if

			else


				sx = 10
				sy = 57 + ff * 48

				sys_print videobuffer, "Empty", 160 - 5 * 4, sy, 0

			end if


		next


		'------------------------------------------




		if currow = 0 then
			rcDest.y = 18
			if curcol = 0 then rcDest.x = 10
			if curcol = 1 then rcDest.x = 108
			if curcol = 2 then rcDest.x = 170
			if curcol = 3 then rcDest.x = 230
			rcDest.x = rcDest.x + 2 + 2 * sin(3.14159 * 2 * itemyloc / 16)
		end if

		if currow > 0 then
			rcDest.x = 0 + 2 * sin(3.14159 * 2 * itemyloc / 16)
			rcDest.y = 53 + (currow - 1) * 48
		end if

		SDL_BlitSurface itemimg(15), null, videobuffer, @rcDest


		if lowerlock = 1 then
			rcDest.y = 18
			if curcol = 1 then rcDest.x = 108
			if curcol = 2 then rcDest.x = 170
			rcDest.x = rcDest.x '+ 2 + 2 * sin(-3.14159 * 2 * itemyloc / 16)

			SDL_BlitSurface itemimg(15), null, videobuffer, @rcDest
		end if



		yy = 255
		if ticks < ticks1 + 1000 then
			yy = 255 * ((ticks - ticks1) / 1000)
			if yy < 0 then yy = 0
			if yy > 255 then yy = 255
		end if

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, yy

		SDL_BlitSurface videobuffer, null, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if



		clouddeg = clouddeg + .01 * fpsr
		while clouddeg >= 360: clouddeg = clouddeg - 360: wend


		itemyloc = itemyloc + .6 * fpsr
		while itemyloc >= 16: itemyloc = itemyloc - 16: wend


		sleep 2
	loop
	SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 64


end sub


sub game_showlogos ()

	dim y as single

	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 320
	rcSrc.h = 240

	rc.x = 0
	rc.y = 0

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY
	rc2.w = 320
	rc2.h = 240

	ticks = SDL_GetTicks
	ticks1 = ticks

	y = 0


	do



		rc.x = 0
		rc.y = 0
		rc.w = 320
		rc.h = 240

		y = 255
		if ticks < ticks1 + 1000 then
			y = 255 * ((ticks - ticks1) / 1000)
			if y < 0 then y = 0
			if y > 255 then y = 255
		end if

		if ticks > ticks1 + 3000 then
			y = 255 - 255 * ((ticks - ticks1 - 3000) / 1000)
			if y < 0 then y = 0
			if y > 255 then y = 255
		end if

		SDL_FillRect videobuffer, @rc, 0

		SDL_SetAlpha logosimg, 0 OR SDL_SRCALPHA, y
		SDL_BlitSurface logosimg, null, videobuffer, null
		SDL_SetAlpha logosimg, 0 OR SDL_SRCALPHA, 255


		SDL_BlitSurface videobuffer, @rc, video, @rc2

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if


		if ticks > ticks1 + 4000 then exit do

loop


end sub



sub game_swash

	dim y as single

	rcDest.x = 0
	rcDest.y = 0
	rcDest.w = 320
	rcDest.h = 240

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY

	y = 0

	do

		y = y + 1 * fpsr

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, y

		SDL_FillRect videobuffer, @rcDest, 0

		SDL_BlitSurface videobuffer, @rcDest, video, @rc2

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if



		clouddeg = clouddeg + .01 * fpsr
		while clouddeg >= 360: clouddeg = clouddeg - 360: wend


		if y > 10 then exit do

	loop

	y = 0
	do

		y = y + 1 * fpsr

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, y * 25

		SDL_BlitSurface mapbg, null, videobuffer, null

		if cloudson = 1 then
			rcDest.x = 256 + 256 * cos(3.141592 / 180 * clouddeg)
			rcDest.y = 192 + 192 * sin(3.141592 / 180 * clouddeg)
			rcDest.w = 320
			rcDest.h = 240

			SDL_BlitSurface cloudimg, @rcDest, videobuffer, null
		end if

		SDL_BlitSurface videobuffer, null, video, @rc2
		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if



		clouddeg = clouddeg + .01 * fpsr
		while clouddeg >= 360: clouddeg = clouddeg - 360: wend


		if y > 10 then exit do

	loop


	SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

end sub

sub game_theend ()

	dim y as single

	rcDest.x = 0
	rcDest.y = 0
	rcDest.w = 320
	rcDest.h = 240

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY


	y = 0

   	for i = 0 to maxfloat

		floattext(i, 0) = 0
		floaticon(i, 0) = 0

	next



	do

		y = y + 1 * fpsr

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, y

		SDL_FillRect videobuffer, @rcDest, 0

		SDL_BlitSurface videobuffer, @rcDest, video, @rc2

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if


		if y > 100 then exit do

	loop


	game_title 0

end sub


sub game_title (mode%)



	dim xofs as single

	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 320
	rcSrc.h = 240

	SDL_FillRect videobuffer2, @rcSrc, 0
	SDL_FillRect videobuffer3, @rcSrc, 0


	rc.x = 0
	rc.y = 0

	rc2.x = SCR_TOPX
	rc2.y = SCR_TOPY
	rc2.w = 320
	rc2.h = 240


	x = 160 - 4 * len(stri$)


	pauseticks = ticks + 500
	bticks = ticks


	SDL_BlitSurface videobuffer, null, videobuffer3, null
	SDL_BlitSurface video, @rc2, videobuffer2, null

	cursel = 0

	keypause = ticks + 220

	ticks1 = ticks

	if menabled = 1 and opmusic = 1 then
		FSOUND_setVolume(musicchannel, 0)
		FSOUND_setPaused(musicchannel, true)

		menuchannel = FSOUND_playSound(FSOUND_FREE, mmenu)
		FSOUND_setPaused(menuchannel, false)
		FSOUND_setVolume(menuchannel, opmusicvol)
		pmenu = 1
	end if


	ldstop = 0
	do



		ld! = ld! + 4 * fpsr
		if ld! > opmusicvol then ld! = opmusicvol
		if menabled = 1 and ldstop = 0 then
			FSOUND_setVolume(menuchannel, ld!)
			if ld! = opmusicvol then ldstop = 1
		end if


 		rc.x = -xofs
		rc.y = 0

		SDL_BlitSurface titleimg, null, videobuffer, @rc

 		rc.x = -xofs + 320
		rc.y = 0

		SDL_BlitSurface titleimg, null, videobuffer, @rc

 		rc.x = 0
		rc.y = 0

		SDL_BlitSurface titleimg2, null, videobuffer, @rc

		y = 172
		x = 160 - 14 * 4
		sys_print videobuffer, "new game/save/load", x, y, 4
		sys_print videobuffer, "options", x, y + 16, 4
		sys_print videobuffer, "quit game", x, y + 32, 4
		if mode = 1 then sys_print videobuffer, "return", x, y + 48, 4

		rc.x = x - 16 - 4 * cos(3.14159 * 2 * itemyloc / 16)
		rc.y = y - 4 + 16 * cursel

		SDL_BlitSurface itemimg(15), null, videobuffer, @rc

		rc.x = 0
		rc.y = 0
		rc.w = 320
		rc.h = 240

		y = 255
		if ticks < ticks1 + 1000 then
			y = 255 * ((ticks - ticks1) / 1000)
			if y < 0 then y = 0
			if y > 255 then y = 255
		end if

		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, y
		SDL_BlitSurface videobuffer, @rc, video, @rc2
		SDL_SetAlpha videobuffer, 0 OR SDL_SRCALPHA, 255

		SDL_Flip video
		SDL_PumpEvents

		tickspassed = ticks
		ticks = SDL_GetTicks

		tickspassed = ticks - tickspassed
		fpsr = tickspassed / 24

		fp = fp + 1
		if ticks > nextticks then
			nextticks = ticks + 1000
			fps = fp
			fp = 0
		end if

		add! = .5 * fpsr
		if add! > 1 then add! = 1
		xofs = xofs + add!
		if xofs >= 320 then xofs = xofs - 320

		itemyloc = itemyloc + .75 * fpsr
		while itemyloc >= 16: itemyloc = itemyloc - 16: wend


		if keypause < ticks then
			SDL_PollEvent(@event)
			keys = SDL_GetKeyState(null)

			if event.type = SDL_KEYDOWN then
				keypause = ticks + 150
				if keys[SDLK_ESCAPE] and mode = 1 then exit do
				if keys[SDLK_UP] then cursel = cursel - 1
				if keys[SDLK_DOWN] then cursel = cursel + 1
				if keys[SDLK_SPACE] or keys[SDLK_RETURN] then
					if cursel = 0 then
						game_saveloadnew
						keypause = ticks + 150
						ticks1 = ticks
					end if
					if cursel = 1 then
						game_configmenu
						keypause = ticks + 150
						ticks1 = ticks
					end if
					if cursel = 2 then

						FSOUND_Close
						SDL_Quit

						end 1
					end if
					if cursel = 3 then exit do
				end if

				if cursel = -1 and mode = 1 then cursel = 3
				if cursel = -1 and mode = 0 then cursel = 2
				if cursel = 4 then cursel = 0
				if cursel = 3 and mode = 0 then cursel = 0

			end if
		end if


	loop

		itemticks = ticks + 210

	if menabled = 1 and opmusic = 1 then
		FSOUND_stopSound(menuchannel)
		FSOUND_setPaused(musicchannel, false)
		FSOUND_setVolume(musicchannel, opmusicvol)
		pmenu = 0
	end if


end sub


sub game_updanims ()


	FOR i = 0 TO lastobj

		nframes = objectinfo(i, 0)
		oanimspd = objectinfo(i, 3)
		frame! = objectframe(i, 0)
		cframe = objectframe(i, 1)
		objectinfo(i, 6) = 0

		IF nframes > 1 THEN

			frame! = frame! + oanimspd / 50 * fpsr
			WHILE frame! >= nframes
				frame! = frame! - nframes
			WEND
			cframe = INT(frame!)
			IF cframe > nframes THEN cframe = nframes - 1
			IF cframe < 0 THEN cframe = 0

			objectframe(i, 0) = frame!
			objectframe(i, 1) = cframe
		END IF


	NEXT


END SUB

sub game_updatey()

	for i = 0 to 2400
		ysort(i) = -1
	next


	ff = int(player.py * 10)
	player.ysort = ff
	ysort(ff) = 0

	firsty = 2400
	lasty = 0

	for i = 1 to lastnpc
		yy = int(npcinfo(i).y * 10)

		do
			if ysort(yy) = -1 or yy = 2400 then exit do
			yy = yy + 1
		loop

		ysort(yy) = i
		if yy < firsty then firsty = yy
		if yy > lasty then lasty = yy

	next

end sub

sub game_updmusic ()

	iplaysound = 0
	if menabled = 1 and opmusic = 1 then

		'if curmap > 5 and curmap < 42 then iplaysound = macademy
		'if curmap > 47 then iplaysound = mgardens
		iplaysound = mgardens
		if roomlock = 1 then iplaysound = mboss

		if iplaysound = mboss and pboss then iplaysound = 0
		if iplaysound = mgardens and pgardens then iplaysound = 0

		if iplaysound > 0 then
			FSOUND_StopSound(musicchannel)

			pboss = 0
			pgardens = 0
			ptown = 0
			pacademy = 0
			pcitadel = 0

			if iplaysound = mboss then pboss = 1
			if iplaysound = mgardens then pgardens = 1

			musicchannel = FSOUND_PlaySound(FSOUND_FREE, iplaysound)
			FSOUND_setVolume(musicchannel, opmusicvol)
		else

			if not FSOUND_isPlaying(musicchannel) then
				loopseta = loopseta + 1
				if loopseta = 4 then loopseta = 0

				if pgardens = 1 then
					FSOUND_StopSound(musicchannel)
					if pgardens = 1 and loopseta = 0 then musicchannel = FSOUND_PlaySound(FSOUND_FREE, mgardens)
					if pgardens = 1 and loopseta = 1 then musicchannel = FSOUND_PlaySound(FSOUND_FREE, mgardens2)
					if pgardens = 1 and loopseta = 2 then musicchannel = FSOUND_PlaySound(FSOUND_FREE, mgardens3)
					if pgardens = 1 and loopseta = 3 then musicchannel = FSOUND_PlaySound(FSOUND_FREE, mgardens4)
				end if

				FSOUND_setVolume(musicchannel, opmusicvol)
			end if

		end if
	end if


end sub


sub game_updnpcs ()

	dim npx as single, npy as single, onpx as single, onpy as single
	dim temp as uinteger ptr, dq as uinteger, bgc as uinteger
	dim nnxa as single, nnya as single, nnxb as single, nnyb as single

	FOR i = 1 TO lastnpc

		IF npcinfo(i).hp > 0 THEN

			' is npc walking

			pass = 0
			if npcinfo(i).attacking = 0 then pass = 1

			if npcinfo(i).spriteset = 5 then pass = 1

			IF pass = 1 THEN

				moveup = 0
				movedown = 0
				moveleft = 0
				moveright = 0

				npx = npcinfo(i).x
				npy = npcinfo(i).y

				onpx = npx
				onpy = npy

				wspd! = npcinfo(i).walkspd / 4

				if npcinfo(i).spriteset = 10 then wspd! = wspd! * 2
				wdir = npcinfo(i).walkdir

				mode = npcinfo(i).movementmode

				xdif = player.px - npx
				ydif = player.py - npy

				if abs(xdif) < 4 * 16 and abs(ydif) < 4 * 16 and mode < 3 then mode = 0
				if npcinfo(i).hp < npcinfo(i).maxhp * .25 then mode = 3

				if npcinfo(i).pause > ticks then mode = -1
				if npcinfo(i).spriteset = 2 and npcinfo(i).castpause > ticks then mode = -1

				if mode = 3 then
					mode = 1
					if abs(xdif) < 4 * 16 and abs(ydif) < 4 * 16 then mode = 3
				end if

				checkpass = 0


				'npc  AI CODE
				'--------------

				'*** aggressive
				IF mode = 0 THEN

					wspd! = npcinfo(i).walkspd / 2

					xdif = player.px - npx
					ydif = player.py - npy

					IF ABS(xdif) > ABS(ydif) THEN
						IF xdif < 4 THEN wdir = 2
						IF xdif > -4 THEN wdir = 3
					ELSE
						IF ydif < 4 THEN wdir = 0
						IF ydif > -4 THEN wdir = 1
					END IF

					IF xdif < 4 THEN moveleft = 1
					IF xdif > -4 THEN moveright = 1
					IF ydif < 4 THEN moveup = 1
					IF ydif > -4 THEN movedown = 1
				END IF
				'*******************


				'*** defensive
				IF mode = 1 THEN

					movingdir = npcinfo(i).movingdir

					IF npcinfo(i).ticks > ticks + 100000 THEN npcinfo(i).ticks = ticks

					IF npcinfo(i).ticks < ticks THEN
						npcinfo(i).ticks = ticks + 2000
						movingdir = INT(RND * 8)
						npcinfo(i).movingdir = movingdir
					END IF

					IF movingdir = 0 THEN
						wdir = 2'left
						moveup = 1
						moveleft = 1
					ELSEIF movingdir = 1 THEN
						wdir = 0'up
						moveup = 1
					ELSEIF movingdir = 2 THEN
						wdir = 3'right
						moveup = 1
						moveright = 1
					ELSEIF movingdir = 3 THEN
						wdir = 3'right
						moveright = 1
					ELSEIF movingdir = 4 THEN
						wdir = 3'right
						moveright = 1
						movedown = 1
					ELSEIF movingdir = 5 THEN
						wdir = 1'down
						movedown = 1
					ELSEIF movingdir = 6 THEN
						wdir = 2'left
						movedown = 1
						moveleft = 1
					ELSEIF movingdir = 7 THEN
						wdir = 2'left
						moveleft = 1
					END IF

					checkpass = 1

				END IF
				'*******************


				'*** run away
				IF mode = 3 THEN

					wspd! = npcinfo(i).walkspd / 2

					xdif = player.px - npx
					ydif = player.py - npy

					IF ABS(xdif) > ABS(ydif) THEN
						IF xdif < 4 THEN wdir = 3
						IF xdif > -4 THEN wdir = 2
					ELSE
						IF ydif < 4 THEN wdir = 1
						IF ydif > -4 THEN wdir = 0
					END IF

					IF xdif < 4 THEN moveright = 1
					IF xdif > -4 THEN moveleft = 1
					IF ydif < 4 THEN movedown = 1
					IF ydif > -4 THEN moveup = 1
				END IF
				'*******************




				'--------------
				movinup = 0
				movindown = 0
				movinleft = 0
				movinright = 0

				xp = (npx / 2 + 6)
				yp = (npy / 2 + 10)

				if npcinfo(i).spriteset = 10 then wspd! = wspd! * 2

				ii! = wspd! * fpsr
				IF ii! < 1 THEN ii! = 1

				d = SDL_LockSurface (clipbg)

				IF moveup THEN
					sx = xp
					sy = yp - ii!
					temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
					dq = *temp
					if npcinfo(i).spriteset = 11 then dq = 0

					IF dq = 0 THEN movinup = 1
					IF dq > 0 THEN
						sx = xp - ii!
						sy = yp - ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movinup = 1
							movinleft = 1
						END IF
					END IF
					IF dq > 0 THEN
						sx = xp + ii!
						sy = yp - ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movinup = 1
							movinright = 1
						END IF
					END IF
				END IF

				IF movedown THEN
					sx = xp
					sy = yp + ii!
					temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
					dq = *temp
					if npcinfo(i).spriteset = 11 then dq = 0

					IF dq = 0 THEN movindown = 1
					IF dq > 0 THEN
						sx = xp - ii!
						sy = yp + ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movindown = 1
							movinleft = 1
						END IF
					END IF
					IF dq > 0 THEN
						sx = xp + ii!
						sy = yp + ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0  THEN
							movindown = 1
							movinright = 1
						END IF
					END IF
				END IF


				IF moveleft THEN
					sx = xp - ii!
					sy = yp
					temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
					dq = *temp
					if npcinfo(i).spriteset = 11 then dq = 0

					IF dq = 0 THEN movinleft = 1
					IF dq > 0 THEN
						sx = xp - ii!
						sy = yp - ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movinleft = 1
							movinup = 1
						END IF
					END IF
					IF dq > 0 THEN
						sx = xp - ii!
						sy = yp + ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movinleft = 1
							movindown = 1
						END IF
					END IF
				END IF


				IF moveright THEN
					sx = xp + ii!
					sy = yp
					temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
					dq = *temp
					if npcinfo(i).spriteset = 11 then dq = 0

					IF dq = 0 THEN movinright = 1
					IF dq > 0 THEN
						sx = xp + ii!
						sy = yp - ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movinright = 1
							movinup = 1
						END IF
					END IF
					IF dq > 0 THEN
						sx = xp + ii!
						sy = yp + ii!
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp
						if npcinfo(i).spriteset = 11 then dq = 0

						IF dq = 0 THEN
							movinright = 1
							movindown = 1
						END IF
					END IF
				END IF

				IF movinup THEN npy = npy - wspd! * fpsr
				IF movindown THEN npy = npy + wspd! * fpsr
				IF movinleft THEN npx = npx - wspd! * fpsr
				IF movinright THEN npx = npx + wspd! * fpsr

				IF checkpass = 1 THEN
					pass = 0
					IF npx >= npcinfo(i).x1 * 16 - 8 AND npx <= npcinfo(i).x2 * 16 + 8 AND npy >= npcinfo(i).y1 * 16 - 8 AND npy <= npcinfo(i).y2 * 16 + 8 THEN pass = 1

					IF pass = 0 THEN
						npx = onpx
						npy = onpy

						npcinfo(i).ticks = ticks
					END IF
				END IF

				aspd! = wspd!

				if npcinfo(i).spriteset = 10 then aspd! = wspd! / 2

				xp = (npx / 2 + 6)
				yp = (npy / 2 + 10)

				sx = xp
				sy = yp
				temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
				bgc = *temp

				SDL_UnLockSurface clipbg

				anpx = npx + 12
				anpy = npy + 20

				lx = (anpx - (anpx MOD 16)) / 16
				ly = (anpy - (anpy MOD 16)) / 16

				if triggerloc(lx, ly) > -1 then bgc = 1
				if npcinfo(i).spriteset = 11 then bgc = 0

				rst = 0

				if npcinfo(i).spriteset = 11 then
					if npx < 40 or npx > 280 or npy < 36 or npy > 204 then rst = 1
				end if

				if bgc > 0 or rst = 1 then
					npx = onpx
					npy = onpy
				end if

				npcinfo(i).x = npx
				npcinfo(i).y = npy

				npcinfo(i).walkdir = wdir
				npcinfo(i).moving = 0

				IF npx <> onpx OR npy <> onpy THEN npcinfo(i).moving = 1

				IF npcinfo(i).moving = 1 THEN
					frame! = npcinfo(i).frame
					cframe = npcinfo(i).cframe

					frame! = frame! + aspd! * fpsr
					WHILE frame! >= 16
						frame! = frame! - 16
					WEND

					cframe = INT(frame!)
					IF cframe > 16 THEN cframe = 16 - 1
					IF cframe < 0 THEN cframe = 0

					npcinfo(i).frame = frame!
					npcinfo(i).cframe = cframe
				END IF



				'spriteset1 specific
				if npcinfo(i).spriteset = 1 and npcinfo(i).attackattempt < ticks then
					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then
						npx = npcinfo(i).x
						npy = npcinfo(i).y

						xdif = player.px - npx
						ydif = player.py - npy

						if abs(xdif) < 20 and abs(ydif) < 20 then
							npcinfo(i).attackattempt = ticks + 100
							if int(rnd * 2) = 0 then

								if menabled = 1 and opeffects = 1 then
									snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndenemyhit))
									FSOUND_setVolume(snd, opeffectsvol)
								end if

								npcinfo(i).attacking = 1
								npcinfo(i).attackframe = 0
							end if
						end if

					end if
				end if


				'onewing specific
				if npcinfo(i).spriteset = 2 then


					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then


							npx = npcinfo(i).x
							npy = npcinfo(i).y

							xdif! = player.px - npx
							ydif! = player.py - npy

							if abs(xdif!) < 24 and abs(ydif!) < 24 then
								dist! = sqr(xdif! * xdif! + ydif! * ydif!)

								if (dist!) < 24 then

									if menabled = 1 and opeffects = 1 then
										snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndbite))
										FSOUND_setVolume(snd, opeffectsvol)
									end if

									npcinfo(i).attacking = 1
									npcinfo(i).attackframe = 0

									npcinfo(i).headtargetx(0) = player.px + 12
									npcinfo(i).headtargety(0) = player.py - 4

								end if
							end if

					end if

					dospell = 0

					if npcinfo(i).attacking = 0 and npcinfo(i).castpause < ticks then
						npcinfo(i).swayspd = npcinfo(i).swayspd + npcinfo(i).swayspd / 200 * fpsr
						if npcinfo(i).swayspd > 15 then
							dospell = 1
							npcinfo(i).swayspd = 1
						end if

						'sway code
						npcinfo(i).swayangle = npcinfo(i).swayangle + npcinfo(i).swayspd * fpsr
						if npcinfo(i).swayangle >= 360 then npcinfo(i).swayangle = npcinfo(i).swayangle - 360

						npcinfo(i).headtargetx(0) = npcinfo(i).x + (24 - npcinfo(i).swayspd / 2) * sin(3.14159 / 180 * npcinfo(i).swayangle) + 12
						npcinfo(i).headtargety(0) = npcinfo(i).y - 36 + 16 + 8 * sin(3.14159 * 2 / 180 * npcinfo(i).swayangle)

					end if

					if dospell = 1 then
						npcinfo(i).pause = ticks + 3000
						npcinfo(i).attacknext = ticks + 4500
						npcinfo(i).castpause = ticks + 4500

						game_castspell 3, npcinfo(i).x, npcinfo(i).y, npcinfo(i).x, npcinfo(i).y, i

						npcinfo(i).headtargetx(0) = npcinfo(i).x
						npcinfo(i).headtargety(0) = npcinfo(i).y - 36 + 16

					end if




					'targethead code
					xdif! = npcinfo(i).bodysection(7).x - npcinfo(i).headtargetx(0)
					ydif! = npcinfo(i).bodysection(7).y - npcinfo(i).headtargety(0)


					npcinfo(i).bodysection(7).x = npcinfo(i).bodysection(7).x  - xdif! * .4 * fpsr
					npcinfo(i).bodysection(7).y = npcinfo(i).bodysection(7).y  - ydif! * .4 * fpsr


					npcinfo(i).bodysection(0).x = npcinfo(i).x + 12
					npcinfo(i).bodysection(0).y = npcinfo(i).y + 12

					for f = 6 to 1 step -1
						xdif! = npcinfo(i).bodysection(f + 1).x - npcinfo(i).bodysection(f - 1).x
						ydif! = npcinfo(i).bodysection(f + 1).y - npcinfo(i).bodysection(f - 1).y

						tx! = npcinfo(i).bodysection(f - 1).x + xdif! / 2
						ty! = npcinfo(i).bodysection(f - 1).y + ydif! / 2

						npcinfo(i).bodysection(f).x = npcinfo(i).bodysection(f).x - (npcinfo(i).bodysection(f).x - tx!) / 3
						npcinfo(i).bodysection(f).y = npcinfo(i).bodysection(f).y - (npcinfo(i).bodysection(f).y - ty!) / 3
					next


				end if



				'boss1 specific and blackknight
				if npcinfo(i).spriteset = 3 or  npcinfo(i).spriteset = 4 then


					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then

						npcinfo(i).attacking = 1
						npcinfo(i).attackframe = 0

						game_castspell 1, npcinfo(i).x, npcinfo(i).y, player.px, player.py, i


					end if

					if npcinfo(i).castpause < ticks then

						game_castspell 6, npcinfo(i).x, npcinfo(i).y, player.px, player.py, i

						npcinfo(i).castpause = ticks + 12000

					end if

				end if


				'firehydra specific
				if npcinfo(i).spriteset = 5 then

					npcinfo(i).swayspd = 4

					'sway code
					npcinfo(i).swayangle = npcinfo(i).swayangle + npcinfo(i).swayspd * fpsr
					if npcinfo(i).swayangle >= 360 then npcinfo(i).swayangle = npcinfo(i).swayangle - 360

					for ff = 0 to 2

						if npcinfo(i).hp > 10 * ff * 20 then

							if npcinfo(i).pause < ticks and npcinfo(i).attacking2(ff) = 0 and npcinfo(i).attacknext2(ff) < ticks then


								npx = npcinfo(i).x
								npy = npcinfo(i).y

								xdif! = player.px - npx
								ydif! = player.py - npy

								if abs(xdif!) < 48 and abs(ydif!) < 48 then
									dist! = sqr(xdif! * xdif! + ydif! * ydif!)

									if (dist!) < 36 then

										if menabled = 1 and opeffects = 1 then
											snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndbite))
											FSOUND_setVolume(snd, opeffectsvol)
										end if

										npcinfo(i).attacking = 1
										npcinfo(i).attacking2(ff) = 1
										npcinfo(i).attackframe2(ff) = 0

										npcinfo(i).headtargetx(ff) = player.px + 12
										npcinfo(i).headtargety(ff) = player.py - 4

										npcinfo(i).swayangle = 0

									end if
								end if

							end if

							if npcinfo(i).attacking2(ff) = 0 then
								npcinfo(i).headtargetx(ff) = npcinfo(i).x + 38 * sin(3.14159 / 180 * (npcinfo(i).swayangle + 120 * ff)) + 12
								npcinfo(i).headtargety(ff) = npcinfo(i).y - 46 + 16 + 16 * sin(3.14159 * 2 / 180 * (npcinfo(i).swayangle + 120 * ff))
							end if



							'targethead code
							xdif! = npcinfo(i).bodysection(10 * ff + 9).x - npcinfo(i).headtargetx(ff)
							ydif! = npcinfo(i).bodysection(10 * ff + 9).y - npcinfo(i).headtargety(ff)


							npcinfo(i).bodysection(10 * ff + 9).x = npcinfo(i).bodysection(10 * ff + 9).x  - xdif! * .4 * fpsr
							npcinfo(i).bodysection(10 * ff + 9).y = npcinfo(i).bodysection(10 * ff + 9).y  - ydif! * .4 * fpsr


							npcinfo(i).bodysection(10 * ff).x = npcinfo(i).x + 12 + 8 * cos(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360))
							npcinfo(i).bodysection(10 * ff).y = npcinfo(i).y + 12 + 8 * sin(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360))


							for f = 8 to 1 step -1
								xdif! = npcinfo(i).bodysection(ff * 10 + f + 1).x - npcinfo(i).bodysection(ff * 10 + f - 1).x
								ydif! = npcinfo(i).bodysection(ff * 10 + f + 1).y - npcinfo(i).bodysection(ff * 10 + f - 1).y

								tx! = npcinfo(i).bodysection(ff * 10 + f - 1).x + xdif! / 2
								ty! = npcinfo(i).bodysection(ff * 10 + f - 1).y + ydif! / 2

								npcinfo(i).bodysection(ff * 10 + f).x = npcinfo(i).bodysection(ff * 10 + f).x - (npcinfo(i).bodysection(ff * 10 + f).x - tx!) / 3
								npcinfo(i).bodysection(ff * 10 + f).y = npcinfo(i).bodysection(ff * 10 + f).y - (npcinfo(i).bodysection(ff * 10 + f).y - ty!) / 3
							next

						end if

					next


				end if


				'spriteset6 specific
				if npcinfo(i).spriteset = 6 and npcinfo(i).attackattempt < ticks then
					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then
						npx = npcinfo(i).x
						npy = npcinfo(i).y

						xdif = player.px - npx
						ydif = player.py - npy

						pass = 0
						if abs(xdif) < 48 and abs(ydif) < 6 then pass = 1
						if abs(ydif) < 48 and abs(xdif) < 6 then pass = 2

						if pass > 0 then
							npcinfo(i).attackattempt = ticks + 100
							if int(rnd * 2) = 0 then
								npcinfo(i).attacking = 1
								npcinfo(i).attackframe = 0

								if pass = 1 and xdif < 0 then
									nnxa = npx - 8
									nnya = npy + 4
									nnxb = npx - 48 - 8
									nnyb = npy + 4
								elseif pass = 1 and xdif > 0 then
									nnxa = npx + 16
									nnya = npy + 4
									nnxb = npx + 16 + 48
									nnyb = npy + 4
								elseif pass = 2 and ydif < 0 then
									nnya = npy
									nnxa = npx + 4
									nnyb = npy - 48
									nnxb = npx + 4
								elseif pass = 2 and ydif > 0 then
									nnya = npy + 20
									nnxa = npx + 4
									nnyb = npy + 20 + 48
									nnxb = npx + 4
								end if

								game_castspell 7, nnxa, nnya, nnxb, nnyb, i
							end if
						end if

					end if
				end if


				'wizard1 specific
				if npcinfo(i).spriteset = 7 then

					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then

						npcinfo(i).attacking = 1
						npcinfo(i).attackframe = 0

						game_castspell 9, npcinfo(i).x, npcinfo(i).y, player.px, player.py, i


					end if

					if npcinfo(i).castpause < ticks then

						'game_castspell 6, npcinfo(i).x, npcinfo(i).y, player.px, player.py, i

						'npcinfo(i).castpause = ticks + 12000

					end if

				end if

				'spriteset6 specific
				if npcinfo(i).spriteset = 8 and npcinfo(i).attackattempt < ticks then
					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then
						npx = npcinfo(i).x
						npy = npcinfo(i).y

						xdif = player.px - npx
						ydif = player.py - npy

						pass = 0
						if abs(xdif) < 56 and abs(ydif) < 6 then pass = 1
						if abs(ydif) < 56 and abs(xdif) < 6 then pass = 2

						if pass > 0 then
							npcinfo(i).attackattempt = ticks + 100
							if int(rnd * 2) = 0 then
								npcinfo(i).attacking = 1
								npcinfo(i).attackframe = 0

								if pass = 1 and xdif < 0 then
									nnxa = npx - 8
									nnya = npy + 4
									nnxb = npx - 56 - 8
									nnyb = npy + 4
									npcinfo(i).walkdir = 2
								elseif pass = 1 and xdif > 0 then
									nnxa = npx + 16
									nnya = npy + 4
									nnxb = npx + 16 + 56
									nnyb = npy + 4
									npcinfo(i).walkdir = 3
								elseif pass = 2 and ydif < 0 then
									nnya = npy
									nnxa = npx + 4
									nnyb = npy - 56
									nnxb = npx + 4
									npcinfo(i).walkdir = 0
								elseif pass = 2 and ydif > 0 then
									nnya = npy + 20
									nnxa = npx + 4
									nnyb = npy + 20 + 56
									nnxb = npx + 4
									npcinfo(i).walkdir = 1
								end if

								game_castspell 7, nnxa, nnya, nnxb, nnyb, i
							end if
						end if

					end if
				end if


				'twowing specific
				if npcinfo(i).spriteset = 9 then


					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then


							npx = npcinfo(i).bodysection(7).x
							npy = npcinfo(i).bodysection(7).y

							xdif! = player.px - npx
							ydif! = player.py - npy

							if abs(xdif!) < 24 and abs(ydif!) < 24 then
								dist! = sqr(xdif! * xdif! + ydif! * ydif!)

								if (dist!) < 24 then

									if menabled = 1 and opeffects = 1 then
										snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndbite))
										FSOUND_setVolume(snd, opeffectsvol)
									end if

									npcinfo(i).attacking = 1
									npcinfo(i).attackframe = 0

									npcinfo(i).headtargetx(0) = player.px + 12
									npcinfo(i).headtargety(0) = player.py - 4

								end if
							end if

					end if



					if npcinfo(i).attacking = 0 and npcinfo(i).castpause < ticks then
						npcinfo(i).swayspd = 4

						'sway code
						npcinfo(i).swayangle = npcinfo(i).swayangle + npcinfo(i).swayspd * fpsr
						if npcinfo(i).swayangle >= 360 then npcinfo(i).swayangle = npcinfo(i).swayangle - 360

						npcinfo(i).headtargetx(0) = npcinfo(i).x + (24 - npcinfo(i).swayspd / 2) * sin(3.14159 / 180 * npcinfo(i).swayangle) + 12
						npcinfo(i).headtargety(0) = npcinfo(i).y - 36 + 16 + 8 * sin(3.14159 * 2 / 180 * npcinfo(i).swayangle)

					end if

					if dospell = 1 then
						npcinfo(i).pause = ticks + 3000
						npcinfo(i).attacknext = ticks + 5000
						npcinfo(i).castpause = ticks + 3000

						game_castspell 3, npcinfo(i).x, npcinfo(i).y, npcinfo(i).x, npcinfo(i).y, i

						npcinfo(i).headtargetx(0) = npcinfo(i).x
						npcinfo(i).headtargety(0) = npcinfo(i).y - 36 + 16

					end if




					'targethead code
					xdif! = npcinfo(i).bodysection(7).x - npcinfo(i).headtargetx(0)
					ydif! = npcinfo(i).bodysection(7).y - npcinfo(i).headtargety(0)


					npcinfo(i).bodysection(7).x = npcinfo(i).bodysection(7).x  - xdif! * .4 * fpsr
					npcinfo(i).bodysection(7).y = npcinfo(i).bodysection(7).y  - ydif! * .4 * fpsr


					npcinfo(i).bodysection(0).x = npcinfo(i).x + 12
					npcinfo(i).bodysection(0).y = npcinfo(i).y + 12

					for f = 6 to 1 step -1
						xdif! = npcinfo(i).bodysection(f + 1).x - npcinfo(i).bodysection(f - 1).x
						ydif! = npcinfo(i).bodysection(f + 1).y - npcinfo(i).bodysection(f - 1).y

						tx! = npcinfo(i).bodysection(f - 1).x + xdif! / 2
						ty! = npcinfo(i).bodysection(f - 1).y + ydif! / 2

						npcinfo(i).bodysection(f).x = npcinfo(i).bodysection(f).x - (npcinfo(i).bodysection(f).x - tx!) / 3
						npcinfo(i).bodysection(f).y = npcinfo(i).bodysection(f).y - (npcinfo(i).bodysection(f).y - ty!) / 3
					next


				end if

				'dragon2 specific

				if npcinfo(i).spriteset = 10 and npcinfo(i).attackattempt < ticks then
					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then
						npx = npcinfo(i).x
						npy = npcinfo(i).y

						xdif = player.px - npx
						ydif = player.py - npy

						if abs(xdif) < 32 and abs(ydif) < 32 then
							npcinfo(i).attackattempt = ticks + 100
							if int(rnd * 2) = 0 then

								if menabled = 1 and opeffects = 1 then
									snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndenemyhit))
									FSOUND_setVolume(snd, opeffectsvol)
								end if

								npcinfo(i).attacking = 1
								npcinfo(i).attackframe = 0
							end if
						end if

					end if
				end if


				'endboss specific

				if npcinfo(i).spriteset = 11 and npcinfo(i).attackattempt < ticks then
					if npcinfo(i).attacknext < ticks and npcinfo(i).pause < ticks and npcinfo(i).attacking = 0 then
						npx = npcinfo(i).x
						npy = npcinfo(i).y

						xdif = player.px - npx
						ydif = player.py - npy

						if abs(xdif) < 38 and abs(ydif) < 38 then
							npcinfo(i).attackattempt = ticks + 100
							if int(rnd * 2) = 0 then

								if menabled = 1 and opeffects = 1 then
									snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndice))
									FSOUND_setVolume(snd, opeffectsvol)
								end if

								npcinfo(i).attacking = 1
								npcinfo(i).attackframe = 0
							end if
						end if

					end if
				end if

			END IF


			npx = npcinfo(i).x
			npy = npcinfo(i).y

			xp = (npx / 2 + 6)
			yp = (npy / 2 + 10)

			rcSrc.x = xp - 1
			rcSrc.y = yp - 1
			rcSrc.w = 3
			rcSrc.h = 3

			if npcinfo(i).pause < ticks then SDL_FillRect clipbg, @rcSrc, i


			pass = 0
			if npcinfo(i).attacking = 1 then pass = 1
			if npcinfo(i).spriteset = 5 then
				if npcinfo(i).attacking2(0) = 1 then pass = 1
				if npcinfo(i).attacking2(1) = 1 then pass = 1
				if npcinfo(i).attacking2(2) = 1 then pass = 1
			end if


			if pass = 1 then

				'spriteset1 specific

				if npcinfo(i).spriteset = 1 then

					npcinfo(i).attackframe = npcinfo(i).attackframe + npcinfo(i).attackspd * fpsr
					if npcinfo(i).attackframe >= 16 then
						npcinfo(i).attackframe = 0
						npcinfo(i).attacking = 0
						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
					end if

					npcinfo(i).cattackframe = int(npcinfo(i).attackframe)

					npx = npcinfo(i).x
					npy = npcinfo(i).y

					xdif = player.px - npx
					ydif = player.py - npy

					dist = 10

					if abs(xdif) < dist and abs(ydif) < dist and player.pause < ticks then

						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
						'npcinfo(i).attackframe = 0
						'npcinfo(i).attacking = 0

						damage = npcinfo(i).attackdamage * (.5 + rnd * 1)

						if player.hp > 0 then game_damageplayer damage


					end if


				end if

				if npcinfo(i).spriteset = 2 then

					'targethead code
					xdif! = npcinfo(i).bodysection(7).x - npcinfo(i).headtargetx(0)
					ydif! = npcinfo(i).bodysection(7).y - npcinfo(i).headtargety(0)


					npcinfo(i).bodysection(7).x = npcinfo(i).bodysection(7).x  - xdif! * .4 * fpsr
					npcinfo(i).bodysection(7).y = npcinfo(i).bodysection(7).y  - ydif! * .4 * fpsr


					npcinfo(i).bodysection(0).x = npcinfo(i).x + 12
					npcinfo(i).bodysection(0).y = npcinfo(i).y + 12

					for f = 6 to 1 step -1
						xdif! = npcinfo(i).bodysection(f + 1).x - npcinfo(i).bodysection(f - 1).x
						ydif! = npcinfo(i).bodysection(f + 1).y - npcinfo(i).bodysection(f - 1).y

						tx! = npcinfo(i).bodysection(f - 1).x + xdif! / 2
						ty! = npcinfo(i).bodysection(f - 1).y + ydif! / 2

						npcinfo(i).bodysection(f).x = npcinfo(i).bodysection(f).x - (npcinfo(i).bodysection(f).x - tx!)
						npcinfo(i).bodysection(f).y = npcinfo(i).bodysection(f).y - (npcinfo(i).bodysection(f).y - ty!)
					next



					npcinfo(i).attackframe = npcinfo(i).attackframe + npcinfo(i).attackspd * fpsr
					if npcinfo(i).attackframe >= 16 then
						npcinfo(i).attackframe = 0
						npcinfo(i).attacking = 0
						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
					end if

					npcinfo(i).cattackframe = int(npcinfo(i).attackframe)

					npx = npcinfo(i).bodysection(7).x
					npy = (npcinfo(i).bodysection(7).y + 16)

					xdif = (player.px + 12) - npx
					ydif = (player.py + 12) - npy

					dist = 8

					if abs(xdif) < dist and abs(ydif) < dist and player.pause < ticks then

						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
						'npcinfo(i).attackframe = 0
						'npcinfo(i).attacking = 0


						damage = npcinfo(i).attackdamage * (1 + rnd * .5)

						if player.hp > 0 then game_damageplayer damage


					end if

				end if


				'firehydra
				if npcinfo(i).spriteset = 5 then

					for ff = 0 to 2

						if npcinfo(i).attacking2(ff) = 1 then

							xdif! = npcinfo(i).bodysection(10 * ff + 9).x - npcinfo(i).headtargetx(ff)
							ydif! = npcinfo(i).bodysection(10 * ff + 9).y - npcinfo(i).headtargety(ff)


							npcinfo(i).bodysection(10 * ff + 9).x = npcinfo(i).bodysection(10 * ff + 9).x  - xdif! * .2 * fpsr
							npcinfo(i).bodysection(10 * ff + 9).y = npcinfo(i).bodysection(10 * ff + 9).y  - ydif! * .2 * fpsr

							npcinfo(i).bodysection(10 * ff).x = npcinfo(i).x + 12 + 8 * cos(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360))
							npcinfo(i).bodysection(10 * ff).y = npcinfo(i).y + 12 + 8 * sin(3.141592 * 2 * (itemyloc / 16 + ff * 120 / 360))


							for f = 8 to 1 step -1
								xdif! = npcinfo(i).bodysection(ff * 10 + f + 1).x - npcinfo(i).bodysection(ff * 10 + f - 1).x
								ydif! = npcinfo(i).bodysection(ff * 10 + f + 1).y - npcinfo(i).bodysection(ff * 10 + f - 1).y

								tx! = npcinfo(i).bodysection(ff * 10 + f - 1).x + xdif! / 2
								ty! = npcinfo(i).bodysection(ff * 10 + f - 1).y + ydif! / 2

								npcinfo(i).bodysection(ff * 10 + f).x = npcinfo(i).bodysection(ff * 10 + f).x - (npcinfo(i).bodysection(ff * 10 + f).x - tx!) / 3
								npcinfo(i).bodysection(ff * 10 + f).y = npcinfo(i).bodysection(ff * 10 + f).y - (npcinfo(i).bodysection(ff * 10 + f).y - ty!) / 3
							next


							npcinfo(i).attackframe2(ff) = npcinfo(i).attackframe2(ff) + npcinfo(i).attackspd * fpsr
							if npcinfo(i).attackframe2(ff) >= 16 then
								npcinfo(i).attackframe2(ff) = 0
								npcinfo(i).attacking2(ff) = 0
								npcinfo(i).attacknext2(ff) = ticks + npcinfo(i).attackdelay
							end if

							npcinfo(i).cattackframe = int(npcinfo(i).attackframe)

							npx = npcinfo(i).bodysection(10 * ff + 9).x
							npy = (npcinfo(i).bodysection(10 * ff + 9).y + 16)

							xdif = (player.px + 12) - npx
							ydif = (player.py + 12) - npy

							dist = 8

							if abs(xdif) < dist and abs(ydif) < dist and player.pause < ticks then

								npcinfo(i).attacknext2(ff) = ticks + npcinfo(i).attackdelay
								'npcinfo(i).attackframe2(ff) = 0
								'npcinfo(i).attacking2(ff) = 0


								damage = npcinfo(i).attackdamage * (1 + rnd * .5)

								if player.hp > 0 then game_damageplayer damage


							end if

						end if

					next

				end if

				'twowing specific
				if npcinfo(i).spriteset = 9 then

					'targethead code
					xdif! = npcinfo(i).bodysection(7).x - npcinfo(i).headtargetx(0)
					ydif! = npcinfo(i).bodysection(7).y - npcinfo(i).headtargety(0)


					npcinfo(i).bodysection(7).x = npcinfo(i).bodysection(7).x  - xdif! * .4 * fpsr
					npcinfo(i).bodysection(7).y = npcinfo(i).bodysection(7).y  - ydif! * .4 * fpsr


					npcinfo(i).bodysection(0).x = npcinfo(i).x + 12
					npcinfo(i).bodysection(0).y = npcinfo(i).y + 12

					for f = 6 to 1 step -1
						xdif! = npcinfo(i).bodysection(f + 1).x - npcinfo(i).bodysection(f - 1).x
						ydif! = npcinfo(i).bodysection(f + 1).y - npcinfo(i).bodysection(f - 1).y

						tx! = npcinfo(i).bodysection(f - 1).x + xdif! / 2
						ty! = npcinfo(i).bodysection(f - 1).y + ydif! / 2

						npcinfo(i).bodysection(f).x = npcinfo(i).bodysection(f).x - (npcinfo(i).bodysection(f).x - tx!)
						npcinfo(i).bodysection(f).y = npcinfo(i).bodysection(f).y - (npcinfo(i).bodysection(f).y - ty!)
					next



					npcinfo(i).attackframe = npcinfo(i).attackframe + npcinfo(i).attackspd * fpsr
					if npcinfo(i).attackframe >= 16 then
						npcinfo(i).attackframe = 0
						npcinfo(i).attacking = 0
						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
					end if

					npcinfo(i).cattackframe = int(npcinfo(i).attackframe)

					npx = npcinfo(i).bodysection(7).x
					npy = (npcinfo(i).bodysection(7).y + 16)

					xdif = (player.px + 12) - npx
					ydif = (player.py + 12) - npy

					dist = 8

					if abs(xdif) < dist and abs(ydif) < dist and player.pause < ticks then

						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
						'npcinfo(i).attackframe = 0
						'npcinfo(i).attacking = 0


						damage = npcinfo(i).attackdamage * (1 + rnd * .5)

						if player.hp > 0 then game_damageplayer damage


					end if

				end if


				'dragon 2 specific
				if npcinfo(i).spriteset = 10 then

					npcinfo(i).attackframe = npcinfo(i).attackframe + npcinfo(i).attackspd * fpsr
					if npcinfo(i).attackframe >= 16 then
						npcinfo(i).attackframe = 0
						npcinfo(i).attacking = 0
						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
					end if

					npcinfo(i).cattackframe = int(npcinfo(i).attackframe)

					npx = npcinfo(i).x
					npy = npcinfo(i).y

					xdif = player.px - npx
					ydif = player.py - npy

					dist = 16 + npcinfo(i).attackframe

					if abs(xdif) < dist and abs(ydif) < dist and player.pause < ticks then

						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
						'npcinfo(i).attackframe = 0
						'npcinfo(i).attacking = 0

						damage = npcinfo(i).attackdamage * (.5 + rnd * 1)

						if player.hp > 0 then game_damageplayer damage


					end if


				end if


				'endboss specific
				if npcinfo(i).spriteset = 11 then

					npcinfo(i).attackframe = npcinfo(i).attackframe + npcinfo(i).attackspd * fpsr
					if npcinfo(i).attackframe >= 16 then
						npcinfo(i).attackframe = 0
						npcinfo(i).attacking = 0
						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
					end if

					npcinfo(i).cattackframe = int(npcinfo(i).attackframe)

					npx = npcinfo(i).x
					npy = npcinfo(i).y

					xdif = player.px - npx
					ydif = player.py - npy

					dist = 36

					if abs(xdif) < dist and abs(ydif) < dist and player.pause < ticks then

						npcinfo(i).attacknext = ticks + npcinfo(i).attackdelay
						'npcinfo(i).attackframe = 0
						'npcinfo(i).attacking = 0

						damage = npcinfo(i).attackdamage * (.5 + rnd * 1)

						if player.hp > 0 then game_damageplayer damage


					end if


				end if

			end if


			'-------end fight code

		END IF

	NEXT


end sub


sub game_updspells ()



	dim dq as uinteger, temp as uinteger ptr
	dim foundel(4), npx as single, npy as single
	dim cl1 as long, cl2 as long, cl3 as long
	dim ll(3,1)

	for i = 0 to maxspell

		if spellinfo(i).frame > 0 then

			spellnum = spellinfo(i).spellnum

			'water
			if spellnum = 0 and forcepause = 0 then


				fr! = (32 - spellinfo(i).frame)

				ll(0,0) = -2
				ll(0,1) = -3
				ll(1,0) = 2
				ll(1,1) = -3
				ll(2,0) = -4
				ll(2,1) = -2
				ll(3,0) = 4
				ll(3,1) = -2

				for f = 0 to 3

					if fr! > f * 4 and fr! < f * 4 + 16 then

						alf = 255

						if fr! < f * 4 + 8 then
							fi = ((fr! - f * 4) * 3) mod 4
							rcSrc.x = 32 + fi * 16
							rcSrc.y = 80
							rcSrc.w = 16
							rcSrc.h = 16

							xloc = spellinfo(i).enemyx + 12 + ll(f, 0) * 16
							yloc = spellinfo(i).enemyy + 16 + ll(f, 1) * 16

							rcDest.x = xloc
							rcDest.y = yloc

							alf = 255 * ((fr! - f * 4) / 8)
						end if

						if fr! >= f * 4 + 8 then

							if f = 0 or f = 2 then fi = 0
							if f = 1 or f = 3 then fi = 1
							rcSrc.x = 32 + fi * 16
							rcSrc.y = 80
							rcSrc.w = 16
							rcSrc.h = 16

							xst = spellinfo(i).enemyx + 12 + ll(f, 0) * 16
							yst = spellinfo(i).enemyy + 16 + ll(f, 1) * 16

							xi! = (spellinfo(i).enemyx - xst) * 2 / 8
							yi! = (spellinfo(i).enemyy - yst) * 2 / 8

							fl! = (fr! - f * 4 - 8) / 2
							xloc = xst + xi! * fl! * fl!
							yloc = yst + yi! * fl! * fl!

							rcDest.x = xloc
							rcDest.y = yloc

							alf = 255
						end if

						if xloc > -16 and xloc < 304 and yloc > -16 and yloc < 224 then

							SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, alf
							SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest
							SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

							if spellinfo(i).damagewho = 0 then
								for e = 1 to lastnpc

									xdif = (xloc + 16) - (npcinfo(e).x + 12)
									ydif = (yloc + 16) - (npcinfo(e).y + 12)

									if (abs(xdif) < 16 and abs(ydif) < 16) then
										damage = player.spelldamage * (1 + rnd * .5) * spellinfo(i).strength

										if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then
											game_damagenpc e, damage, 1
											if menabled = 1 and opeffects = 1 then
												snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndics))
												FSOUND_setVolume(snd, opeffectsvol)
											end if

										end if

									end if

								next
							end if


							'check for post damage
							if nposts > 0 then
								for e = 0 to nposts - 1
									xdif = (xloc + 16) - (postinfo(e,0) + 8)
									ydif = (yloc + 16) - (postinfo(e,1) + 8)

									if (abs(xdif) < 16 and abs(ydif) < 16) then
										objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
										objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

										rcSrc.x = postinfo(e,0) / 2
										rcSrc.y = postinfo(e,1) / 2
										rcSrc.w = 8
										rcSrc.h = 8

										t = SDL_FillRect (clipbg2, @rcSrc, 0)

										game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)

										if menabled = 1 and opeffects = 1 then
											snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndics))
											FSOUND_setVolume(snd, opeffectsvol)
										end if

									end if
								next

							end if


						end if

					end if


				next


			end if



			'metal
			if spellnum = 1 and forcepause = 0 then

				fr = int((32 - spellinfo(i).frame)*4) mod 3

				rcSrc.x = fr * 48
				rcSrc.y = 0
				rcSrc.w = 48
				rcSrc.h = 48

				c1! = (32 - spellinfo(i).frame) / 16
				c! = sin(3.14159 * 2 * c1!)


				halfx = (spellinfo(i).homex-12) + ((spellinfo(i).enemyx-12) - (spellinfo(i).homex-12)) / 2
				halfy = (spellinfo(i).homey-12) + ((spellinfo(i).enemyy-12) - (spellinfo(i).homey-12)) / 2

				wdth = (halfx - spellinfo(i).homex) * 1.2
				hight = (halfy - spellinfo(i).homey) * 1.2

				xloc = halfx + wdth * cos(3.14159 + 3.14159 * 2 * c1!)
				yloc = halfy + hight * sin(3.14159 + 3.14159 * 2 * c1!)

				rcDest.x = xloc
				rcDest.y = yloc

				SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest


				spellinfo(i).frame = spellinfo(i).frame - .2 * fpsr
				if spellinfo(i).frame < 0 then spellinfo(i).frame = 0


				if spellinfo(i).damagewho = 0 then
					for e = 1 to lastnpc

						xdif = (xloc + 24) - (npcinfo(e).x + 12)
						ydif = (yloc + 24) - (npcinfo(e).y + 12)

						if (abs(xdif) < 24 and abs(ydif) < 24) then
							damage = player.spelldamage * (1 + rnd * .5) * spellinfo(i).strength

							if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then
								game_damagenpc e, damage, 1
								if menabled = 1 and opeffects = 1 then
									snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndmetalhit))
									FSOUND_setVolume(snd, opeffectsvol)
								end if

							end if

						end if
					next
				end if

				if spellinfo(i).damagewho = 1 then

					'--------- boss 1 specific
					if spellinfo(i).frame = 0 and npcinfo(spellinfo(i).npc).spriteset = 3 then
						npc = spellinfo(i).npc
						npcinfo(npc).attackframe = 0
						npcinfo(npc).attacking = 0

						npcinfo(npc).pause = ticks + 1000
						npcinfo(npc).attacknext = ticks + 4000

					end if
					'---------------


					'--------- blackknight specific
					if spellinfo(i).frame = 0 and npcinfo(spellinfo(i).npc).spriteset = 4 then
						npc = spellinfo(i).npc
						npcinfo(npc).attackframe = 0
						npcinfo(npc).attacking = 0

						npcinfo(npc).pause = ticks + 1000
						npcinfo(npc).attacknext = ticks + 3500

					end if
					'---------------

					xdif = (xloc + 24) - (player.px + 12)
					ydif = (yloc + 24) - (player.py + 12)

					if (abs(xdif) < 24 and abs(ydif) < 24) and player.pause < ticks then
						npx = player.px
						npy = player.py


						damage = npcinfo(spellinfo(i).npc).spelldamage * (1 + rnd * .5)

						if player.hp > 0 then
							game_damageplayer damage
							if menabled = 1 and opeffects = 1 then
								snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndmetalhit))
								FSOUND_setVolume(snd, opeffectsvol)
							end if

						end if


					end if
				end if


				'check for post damage
				if nposts > 0 then
					for e = 0 to nposts - 1
						xdif = (xloc + 24) - (postinfo(e,0) + 8)
						ydif = (yloc + 24) - (postinfo(e,1) + 8)

						if (abs(xdif) < 24 and abs(ydif) < 24) then
							objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
							objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

							rcSrc.x = postinfo(e,0) / 2
							rcSrc.y = postinfo(e,1) / 2
							rcSrc.w = 8
							rcSrc.h = 8

							t = SDL_FillRect (clipbg2, @rcSrc, 0)

							game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)

							if menabled = 1 and opeffects = 1 then
								snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndmetalhit))
								FSOUND_setVolume(snd, opeffectsvol)
							end if

						end if
					next

				end if

			end if


			'earth
			if spellnum = 2 and forcepause = 0 then

				hght = 240 - spellinfo(i).enemyy

				for f = 8 to 0 step -1

					fr! = (32 - spellinfo(i).frame)

					if fr! > f and fr! < f + 16 then

						rcSrc.x = 32 * spellinfo(i).rockimg(f)
						rcSrc.y = 48
						rcSrc.w = 32
						rcSrc.h = 32

						scatter = 0
						if fr! < 8 + f then
							xloc = spellinfo(i).enemyx - 4
							yloc = spellinfo(i).enemyy * (1 - cos(3.14159 / 2 * (fr! - f) / 8)) ^ 2
						else
							scatter = 1
							xloc = spellinfo(i).enemyx - 4 - spellinfo(i).rockdeflect(f) * sin(3.14159 / 2* ((fr! - f) - 8) / 8)
							yloc = spellinfo(i).enemyy + hght * (1 - cos(3.14159 / 2 * ((fr! - f) - 8) / 8))
						end if

						rcDest.x = xloc
						rcDest.y = yloc

						if xloc > -16 and xloc < 304 and yloc > -16 and yloc < 224 then

							SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

							if scatter = 1 then
								if spellinfo(i).damagewho = 0 then
									for e = 1 to lastnpc

										xdif = (xloc + 16) - (npcinfo(e).x + 12)
										ydif = (yloc + 16) - (npcinfo(e).y + 12)

										if (abs(xdif) < 16 and abs(ydif) < 16) then
											damage = player.spelldamage * (1 + rnd * .5) * spellinfo(i).strength

											if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then
												game_damagenpc e, damage, 1
												if menabled = 1 and opeffects = 1 then
													snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndrocks))
													FSOUND_setVolume(snd, opeffectsvol)
												end if

											end if

										end if

									next
								end if


								'check for post damage
								if nposts > 0 then
									for e = 0 to nposts - 1
										xdif = (xloc + 16) - (postinfo(e,0) + 8)
										ydif = (yloc + 16) - (postinfo(e,1) + 8)

										if (abs(xdif) < 16 and abs(ydif) < 16) then
											objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
											objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

											rcSrc.x = postinfo(e,0) / 2
											rcSrc.y = postinfo(e,1) / 2
											rcSrc.w = 8
											rcSrc.h = 8

											t = SDL_FillRect (clipbg2, @rcSrc, 0)

											game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)

											if menabled = 1 and opeffects = 1 then
												snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndrocks))
												FSOUND_setVolume(snd, opeffectsvol)
											end if

										end if
									next

								end if


							end if
						end if

					end if

				next

				spellinfo(i).frame = spellinfo(i).frame - .2 * fpsr
				if spellinfo(i).frame < 0 then spellinfo(i).frame = 0


			end if




			'crystal
			if spellnum = 5 then

				fra = (32 - spellinfo(i).frame)
				fr = ((spellinfo(i).frame) * 2) mod 8

				rcSrc.x = fr * 32
				rcSrc.y = 96 + 48
				rcSrc.w = 32
				rcSrc.h = 64

				rcDest.x = player.px - 4
				rcDest.y = player.py + 16 - 48

				f = 160
				if fra < 8 then f = 192 * fra/ 8
				if fra > 24 then f = 192 * (1 - (fra - 24) / 8)

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, f

				SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

				spellinfo(i).frame = spellinfo(i).frame - .3 * fpsr
				if spellinfo(i).frame < 0 then
					spellinfo(i).frame = 0
					forcepause = 0

					npx = player.px + 12
					npy = player.py + 20

					lx = (npx - (npx MOD 16)) / 16
					ly = (npy - (npy MOD 16)) / 16


					for f = 0 to 4
						foundel(f) = 0
					next

					for xo = -2 to 2
						for yo = -2 to 2

							sx = lx + xo
							sy = ly + yo

							IF sx > -1 AND sx < 20 AND sy > -1 AND sy < 15 THEN

								for l = 0 to 2
									curtile = tileinfo(l, sx, sy, 0)
									curtilel = tileinfo(l, sx, sy, 1)

									IF curtile > 0 THEN
										curtile = curtile - 1
										curtilex = curtile MOD 20
										curtiley = (curtile - curtilex) / 20

										element = elementmap(curtilex, curtiley)
										if (element > -1 and curtilel = 0) then foundel(element) = 1

									end if
								next

								o = objmap(sx, sy)
								if o > -1 then
									if objectinfo(o, 4) = 1 then foundel(1) = 1
									if o = 1 or o = 2 then
										foundel(1) = 1
										foundel(3) = 1
									end if
								end if

							end if

						next
					next

					t$ = "nothing..."
					f = 0
					do
						if foundel(f) = 1 and player.foundspell(f) = 0 then
							player.foundspell(f) = 1
							player.spellcharge(f) = 0
							if f = 0 then t$ = "Water Essence"
							if f = 1 then t$ = "Metal Essence"
							if f = 2 then t$ = "Earth Essence"
							if f = 3 then t$ = "Fire Essence"
							exit do
						end if

						f=f+1
						if f = 5 then exit do
					loop

					game_eventtext "Found..." + t$
				end if



			end if



			'room fireballs
			if spellnum = 6 and forcepause = 0 then

				if spellinfo(i).frame > 16 then

					fr! = (32 - spellinfo(i).frame)

					SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 192 * sin(3.14159 * fr! / 4)

					rcSrc.x = 16 * int(rnd * 2)
					rcSrc.y = 80
					rcSrc.w = 16
					rcSrc.h = 16


					for ff = 0 to spellinfo(i).nfballs - 1

						xloc = spellinfo(i).fireballs(ff,0) + int(rnd * 3) - 1
						yloc = spellinfo(i).fireballs(ff,1) + int(rnd * 3) - 1


						rcDest.x = xloc
						rcDest.y = yloc


						SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest
					next

					SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

				else


					SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 192


					rcSrc.x = 16 * int(rnd * 2)
					rcSrc.y = 80
					rcSrc.w = 16
					rcSrc.h = 16


					for ff = 0 to spellinfo(i).nfballs - 1

						ax! = spellinfo(i).fireballs(ff,0)
						ay! = spellinfo(i).fireballs(ff,1)
						bx! = player.px + 4
						by! = player.py + 4
						d! = sqr((bx! - ax!) ^ 2 + (by! - ay!) ^2)

						tx! = (bx! - ax!) / d!
						ty! = (by! - ay!) / d!

						spellinfo(i).fireballs(ff,2) = spellinfo(i).fireballs(ff,2) + tx! * 1.2* fpsr
						spellinfo(i).fireballs(ff,3) = spellinfo(i).fireballs(ff,3) + ty! * 1.2 * fpsr

						if spellinfo(i).ballon(ff) = 1 then

							spellinfo(i).fireballs(ff,0) = ax! + spellinfo(i).fireballs(ff,2) * .2 * fpsr
							spellinfo(i).fireballs(ff,1) = ay! + spellinfo(i).fireballs(ff,3) * .2 * fpsr

							xloc = spellinfo(i).fireballs(ff,0) + int(rnd * 3) - 1
							yloc = spellinfo(i).fireballs(ff,1) + int(rnd * 3) - 1

							rcDest.x = xloc
							rcDest.y = yloc

							SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest
						end if

						if xloc < -1 or yloc < -1 or xloc > 304 or yloc > 224 then spellinfo(i).ballon(ff) = 0
					next

					SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255



				end if




				spellinfo(i).frame = spellinfo(i).frame - .2 * fpsr
				if spellinfo(i).frame < 0 then spellinfo(i).frame = 0


				if spellinfo(i).damagewho = 1 then


					for ff = 0 to spellinfo(i).nfballs - 1

						if spellinfo(i).ballon(ff) = 1 then

							xloc = spellinfo(i).fireballs(ff,0) + int(rnd * 3) - 1
							yloc = spellinfo(i).fireballs(ff,1) + int(rnd * 3) - 1

							xdif = (xloc + 8) - (player.px + 12)
							ydif = (yloc + 8) - (player.py + 12)

							if (abs(xdif) < 8 and abs(ydif) < 8) and player.pause < ticks then

								damage = npcinfo(spellinfo(i).npc).spelldamage * (1 + rnd * .5) / 3

								if player.hp > 0 then game_damageplayer damage

								if menabled = 1 and opeffects = 1 then
									 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
									 FSOUND_setVolume(snd, opeffectsvol)
								end if

							end if
						end if
					next
				end if



			end if

			'lightning bomb
			if spellnum = 8 then

				cl1 = SDL_MapRGB(videobuffer->format, 0, 32, 204)
				cl2 = SDL_MapRGB(videobuffer->format, 142, 173, 191)
				cl3 = SDL_MapRGB(videobuffer->format, 240, 240, 240)

				px = player.px + 12
				py = player.py + 12

				apx = px + int(rnd * 5 - 2)
				apy = py + int(rnd * 5 - 2)

				for f = 0 to 0

					y = apy
					for x = apx to 319

						rn = int(rnd * 3)

						if orn = 0 then y = y -1
						if orn = 2 then y = y +1

						sys_line videobuffer,x, y - 1, x, y + 2, cl1
						sys_line videobuffer,x, y, x, y + 1, cl3

						if rn = 0 then sys_line videobuffer, x, y + 1, x, y + 1, cl2
						if rn = 2 then sys_line videobuffer, x, y, x, y, cl2

						orn = rn

						if spellinfo(i).damagewho = 0 then
							for e = 1 to lastnpc

								xdif = (x + 16) - (npcinfo(e).x + 12)
								ydif = (y + 16) - (npcinfo(e).y + 12)

								if (abs(xdif) < 8 and abs(ydif) < 8) then
									damage = 30 * (1 + rnd * .5)

									if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then game_damagenpc e, damage, 1
								end if

							next
						end if
						'check for post damage
						if nposts > 0 then
							for e = 0 to nposts - 1
								xdif = (xloc + 16) - (postinfo(e,0) + 8)
								ydif = (yloc + 16) - (postinfo(e,1) + 8)

								if (abs(xdif) < 16 and abs(ydif) < 16) then
									objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
									objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

									rcSrc.x = postinfo(e,0) / 2
									rcSrc.y = postinfo(e,1) / 2
									rcSrc.w = 8
									rcSrc.h = 8

									t = SDL_FillRect (clipbg2, @rcSrc, 0)

									game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)
								end if
							next

						end if
					next


					y = apy
					for x = apx to 0 step -1

						rn = int(rnd * 3)

						if orn = 0 then y = y -1
						if orn = 2 then y = y +1

						sys_line videobuffer,x, y - 1, x, y + 2, cl1
						sys_line videobuffer,x, y, x, y + 1, cl3

						if rn = 0 then sys_line videobuffer, x, y + 1, x, y + 1, cl2
						if rn = 2 then sys_line videobuffer, x, y, x, y, cl2

						orn = rn

						if spellinfo(i).damagewho = 0 then
							for e = 1 to lastnpc

								xdif = (x + 16) - (npcinfo(e).x + 12)
								ydif = (y + 16) - (npcinfo(e).y + 12)

								if (abs(xdif) < 8 and abs(ydif) < 8) then
									damage = 30 * (1 + rnd * .5)

									if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then game_damagenpc e, damage, 1
								end if

							next
						end if
						'check for post damage
						if nposts > 0 then
							for e = 0 to nposts - 1
								xdif = (xloc + 16) - (postinfo(e,0) + 8)
								ydif = (yloc + 16) - (postinfo(e,1) + 8)

								if (abs(xdif) < 16 and abs(ydif) < 16) then
									objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
									objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

									rcSrc.x = postinfo(e,0) / 2
									rcSrc.y = postinfo(e,1) / 2
									rcSrc.w = 8
									rcSrc.h = 8

									t = SDL_FillRect (clipbg2, @rcSrc, 0)

									game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)
								end if
							next

						end if
					next

					x = apx
					for y = apy to 239

						rn = int(rnd * 3)

						if orn = 0 then x = x -1
						if orn = 2 then x = x +1

						sys_line videobuffer,x - 1, y, x + 2, y, cl1
						sys_line videobuffer,x, y, x + 1, y, cl3

						if rn = 0 then sys_line videobuffer, x + 1, y, x + 1, y, cl2
						if rn = 2 then sys_line videobuffer, x, y, x, y, cl2

						orn = rn

						if spellinfo(i).damagewho = 0 then
							for e = 1 to lastnpc

								xdif = (x + 16) - (npcinfo(e).x + 12)
								ydif = (y + 16) - (npcinfo(e).y + 12)

								if (abs(xdif) < 8 and abs(ydif) < 8) then
									damage = 30 * (1 + rnd * .5)

									if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then game_damagenpc e, damage, 1
								end if

							next
						end if
						'check for post damage
						if nposts > 0 then
							for e = 0 to nposts - 1
								xdif = (xloc + 16) - (postinfo(e,0) + 8)
								ydif = (yloc + 16) - (postinfo(e,1) + 8)

								if (abs(xdif) < 16 and abs(ydif) < 16) then
									objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
									objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

									rcSrc.x = postinfo(e,0) / 2
									rcSrc.y = postinfo(e,1) / 2
									rcSrc.w = 8
									rcSrc.h = 8

									t = SDL_FillRect (clipbg2, @rcSrc, 0)

									game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)
								end if
							next

						end if
					next


					x = apx
					for y = apy to 0 step - 1

						rn = int(rnd * 3)

						if orn = 0 then x = x -1
						if orn = 2 then x = x +1

						sys_line videobuffer,x - 1, y, x + 2, y, cl1
						sys_line videobuffer,x, y, x + 1, y, cl3

						if rn = 0 then sys_line videobuffer, x + 1, y, x + 1, y, cl2
						if rn = 2 then sys_line videobuffer, x, y, x, y, cl2

						orn = rn

						if spellinfo(i).damagewho = 0 then
							for e = 1 to lastnpc

								xdif = (x + 16) - (npcinfo(e).x + 12)
								ydif = (y + 16) - (npcinfo(e).y + 12)

								if (abs(xdif) < 8 and abs(ydif) < 8) then
									damage = 30 * (1 + rnd * .5)

									if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then game_damagenpc e, damage, 1
								end if

							next
						end if
						'check for post damage
						if nposts > 0 then
							for e = 0 to nposts - 1
								xdif = (xloc + 16) - (postinfo(e,0) + 8)
								ydif = (yloc + 16) - (postinfo(e,1) + 8)

								if (abs(xdif) < 16 and abs(ydif) < 16) then
									objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
									objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

									rcSrc.x = postinfo(e,0) / 2
									rcSrc.y = postinfo(e,1) / 2
									rcSrc.w = 8
									rcSrc.h = 8

									t = SDL_FillRect (clipbg2, @rcSrc, 0)

									game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)
								end if
							next

						end if
					next

				next


				spellinfo(i).frame = spellinfo(i).frame - .5 * fpsr
				if spellinfo(i).frame < 0 then
					spellinfo(i).frame = 0
					forcepause = 0
				end if

			end if

			'wizard 1 lightning
			if spellnum = 9 then

				cl1 = SDL_MapRGB(videobuffer->format, 0, 32, 204)
				cl2 = SDL_MapRGB(videobuffer->format, 142, 173, 191)
				cl3 = SDL_MapRGB(videobuffer->format, 240, 240, 240)

				px = spellinfo(i).enemyx + 12
				py = spellinfo(i).enemyy + 24

				apx = px + int(rnd * 20 - 10)
				apy = py + int(rnd * 20 - 10)

				x = apx
				for y = 0 to apy

					if y < 240 then
						rn = int(rnd * 3)

						if orn = 0 then x = x -1
						if orn = 2 then x = x +1

						sys_line videobuffer,x - 1, y, x + 2, y, cl1
						sys_line videobuffer,x, y, x + 1, y, cl3

						if rn = 0 then sys_line videobuffer, x + 1, y, x + 1, y, cl2
						if rn = 2 then sys_line videobuffer, x, y, x, y, cl2

						orn = rn

						if spellinfo(i).damagewho = 1 then

							xdif = (x + 8) - (player.px + 12)
							ydif = (y + 8) - (player.py + 12)

							if (abs(xdif) < 8 and abs(ydif) < 8) and player.pause < ticks then
								damage = (player.hp * .75) * (rnd * .5 + .5)
								if damage < 5 then damage = 5

								if npcinfo(spellinfo(i).npc).spriteset = 12 then
									if damage < 50 then damage = 40 + int(rnd * 40)
								end if

								if player.hp > 0 then game_damageplayer damage
							end if
						end if
					end if
				next

				spellinfo(i).frame = spellinfo(i).frame - .5 * fpsr
				if spellinfo(i).frame < 0 then
					spellinfo(i).frame = 0

					npcinfo(spellinfo(i).npc).attacking = 0
					npcinfo(spellinfo(i).npc).attacknext = ticks + npcinfo(spellinfo(i).npc).attackdelay
				end if

			end if


		end if



	next


end sub



sub game_updspellsunder ()



	dim dq as uinteger, temp as uinteger ptr
	dim npx as single, npy as single, onpx as single, onpy as single

	if forcepause = 1 then exit sub

	for i = 0 to maxspell

		if spellinfo(i).frame > 0 then

			spellnum = spellinfo(i).spellnum

			'water
			if spellnum = 0 then

				fra = (32 - spellinfo(i).frame)
				fr = ((32 - spellinfo(i).frame) * 2) mod 4

				rcSrc.x = fr * 48
				rcSrc.y = 96
				rcSrc.w = 48
				rcSrc.h = 48

				rcDest.x = spellinfo(i).enemyx - 12
				rcDest.y = spellinfo(i).enemyy - 8

				f = 160
				if fra < 8 then f = 160 * fra/ 8
				if fra > 24 then f = 160 * (1 - (fra - 24) / 8)

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, f

				SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

				spellinfo(i).frame = spellinfo(i).frame - .2 * fpsr
				if spellinfo(i).frame < 0 then spellinfo(i).frame = 0


				for f = 1 to lastnpc
					xdif = spellinfo(i).enemyx - npcinfo(f).x
					ydif = spellinfo(i).enemyy - npcinfo(f).y

					dist = sqr(xdif * xdif + ydif * ydif)

					if dist > 20 then dist = 20

					if dist > 5 then
						xm = 1
						if xdif < 0 then xm = -1
						ym = 1
						if ydif < 0 then ym = -1

						ratio! = (1 - dist / 25)

						newx! = npcinfo(f).x + ratio! * xdif / 3 * fpsr
						newy! = npcinfo(f).y + ratio! * ydif / 3 * fpsr

						sx = (newx! / 2 + 6)
						sy = (newy! / 2 + 10)
						temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
						dq = *temp

						'SDL_UnLockSurface clipbg

						if dq = 0 then
							npcinfo(f).x = newx!
							npcinfo(f).y = newy!
							'npcinfo(f).castpause = ticks + 200
						else

							xpass = 0
							ypass = 0

							sx = (newx! / 2 + 6)
							sy = (npcinfo(f).y / 2 + 10)
							temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
							dq = *temp

							if dq = 0 then xpass = 1


							sx = (npcinfo(f).x / 2 + 6)
							sy = (newy! / 2 + 10)
							temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
							dq = *temp

							if dq = 0 then ypass = 1

							if ypass = 1 then
								newx! = npcinfo(f).x
							elseif xpass = 1 then
								newy! = npcinfo(f).y
							end if

							if xpass = 1 or ypass = 1 then
								npcinfo(f).x = newx!
								npcinfo(f).y = newy!
								'npcinfo(f).castpause = ticks + 200
							end if

						end if
					end if
				next

			end if


			'fire
			if spellnum = 3 then


				fr! = (32 - spellinfo(i).frame)

				fr! = fr! ^ 2 * (1 - cos(3.14159 / 4 + 3.14159 / 4 * fr! / 32))

				if fr! > 32 then fr! = 32

				s = 8
				if spellinfo(i).frame < 8 then s = spellinfo(i).frame

				fra = int(fr!)


				for f = 0 to 4


					for x = 0 to fra step 2

						if spellinfo(i).legalive(f) >= x then

							SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 192 * sin(3.14159 * x / 32) * s / 8

							an = 360 / 5 * f + x / 32 * 180

							rcSrc.x = 16 * int(rnd * 2)
							rcSrc.y = 80
							rcSrc.w = 16
							rcSrc.h = 16

							xloc = spellinfo(i).enemyx + 4 + x * 2 * cos(3.14159 / 180 * an) + int(rnd * 3) - 1
							yloc = spellinfo(i).enemyy + 4 + x * 2 * sin(3.14159 / 180 * an) + int(rnd * 3) - 1


							rcDest.x = xloc
							rcDest.y = yloc

							if xloc > -1 and xloc < 304 and yloc > -1 and yloc < 224 then

								SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

								sx = (xloc / 2 + 4)
								sy = (yloc / 2 + 8)
								temp = 	clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
								dq = *temp

								IF dq > 1000 and x > 4 THEN spellinfo(i).legalive(f) = x

								if spellinfo(i).damagewho = 0 then
									for e = 1 to lastnpc

										xdif = (xloc + 8) - (npcinfo(e).x + 12)
										ydif = (yloc + 8) - (npcinfo(e).y + 12)

										if (abs(xdif) < 8 and abs(ydif) < 8) then
											damage = player.spelldamage * (1 + rnd * .5) * spellinfo(i).strength

											if npcinfo(e).spriteset = 5 then damage = -damage
											if npcinfo(e).spriteset = 11 then damage = -damage
											if npcinfo(e).hp > 0 and npcinfo(e).pause < ticks then
												game_damagenpc e, damage, 1
												if menabled = 1 and opeffects = 1 then
													 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
													 FSOUND_setVolume(snd, opeffectsvol)
												end if
											end if

										end if
									next
								end if

								if spellinfo(i).damagewho = 1 then

									xdif = (xloc + 8) - (player.px + 12)
									ydif = (yloc + 8) - (player.py + 12)

									if (abs(xdif) < 8 and abs(ydif) < 8) and player.pause < ticks then

										damage = npcinfo(spellinfo(i).npc).spelldamage * (1 + rnd * .5)

										if player.hp > 0 then
											game_damageplayer damage

											if menabled = 1 and opeffects = 1 then
												 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
												 FSOUND_setVolume(snd, opeffectsvol)
											end if
										end if

									end if
								end if

								'check for post damage
								if nposts > 0 then
									for e = 0 to nposts - 1
										xdif = (xloc + 8) - (postinfo(e,0) + 8)
										ydif = (yloc + 8) - (postinfo(e,1) + 8)

										if (abs(xdif) < 8 and abs(ydif) < 8) then
											objmapf(curmap, postinfo(e,0) / 16, postinfo(e,1) / 16) = 1
											objmap(postinfo(e,0) / 16, postinfo(e,1) / 16) = -1

											rcSrc.x = postinfo(e,0) / 2
											rcSrc.y = postinfo(e,1) / 2
											rcSrc.w = 8
											rcSrc.h = 8

											t = SDL_FillRect (clipbg2, @rcSrc, 0)

											if menabled = 1 and opeffects = 1 then
												 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
												 FSOUND_setVolume(snd, opeffectsvol)
											end if

											game_addFloatIcon 99, postinfo(e,0), postinfo(e,1)
										end if
									next

								end if



							end if
						end if


					next

				next

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255

				spellinfo(i).frame = spellinfo(i).frame - .2 * fpsr
				if spellinfo(i).frame < 0 then spellinfo(i).frame = 0


			end if


			'sprite 6 spitfire
			if spellnum = 7 then

				xspan! = spellinfo(i).enemyx - spellinfo(i).homex
				yspan! = spellinfo(i).enemyy - spellinfo(i).homey

				fr! = (32 - spellinfo(i).frame)

				for f = 0 to 7

					alpha = 0
					xx! = 0
					if fr! > f * 2 and fr! < f * 2 + 16 then xx! = fr! - f * 2

					if xx! < 8 then alpha = 255 * xx! / 8
					if xx! > 8 then alpha = 255 * (1 - (xx! - 8) / 8)
					yy! = 16 * sin(3.141592 / 2 * xx! / 16) - 8

					if alpha < 0 then alpha = 0
					if alpha > 255 then alpha = 255

					SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, alpha

					rcSrc.x = 16 * int(rnd * 2)
					rcSrc.y = 80
					rcSrc.w = 16
					rcSrc.h = 16

					xloc = spellinfo(i).homex + xspan! / 7 * f
					yloc = spellinfo(i).homey + yspan! / 7 * f - yy!

					rcDest.x = xloc
					rcDest.y = yloc

					if xloc > -16 and xloc < 320 and yloc > -16 and yloc < 240 then
						SDL_BlitSurface spellimg, @rcSrc, videobuffer, @rcDest

						if spellinfo(i).damagewho = 1 then

							xdif = (xloc + 8) - (player.px + 12)
							ydif = (yloc + 8) - (player.py + 12)

							if (abs(xdif) < 8 and abs(ydif) < 8) and player.pause < ticks and alpha > 64 then

								damage = npcinfo(spellinfo(i).npc).spelldamage * (1 + rnd * .5)

								if player.hp > 0 then
									game_damageplayer damage
									if menabled = 1 and opeffects = 1 then
										 snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndfire))
										 FSOUND_setVolume(snd, opeffectsvol)
									end if
								end if

							end if
						end if

					end if

				next

				SDL_SetAlpha spellimg, 0 OR SDL_SRCALPHA, 255
				spellinfo(i).frame = spellinfo(i).frame - .5 * fpsr
				if spellinfo(i).frame < 0 then spellinfo(i).frame = 0

				if spellinfo(i).frame = 0 then
					npcinfo(spellinfo(i).npc).attacking = 0
					npcinfo(spellinfo(i).npc).attacknext = ticks + npcinfo(spellinfo(i).npc).attackdelay
				end if


			end if


		end if



	next


end sub



sub sys_customLoad ()


	dim temp as SDL_Surface ptr

	temp = IMG_Load("art\w.pcx")

	anim = 0

	playeranims = 4
	anims(anim) = SDL_CreateRGBSurface(SDL_SWSURFACE, 24 * 4, 24 * playeranims, SCR_BITS, video->format->rmask, video->format->gmask, video->format->bmask, video->format->amask)

	dim walkframeloc(4)


  walkframeloc(0) = 1
  walkframeloc(1) = 0
  walkframeloc(2) = 1
  walkframeloc(3) = 2

  ymod = 24
  ymod = 0
  FOR a = 0 TO 3
	FOR i = 0 TO 3
	  'dqbget 5, a * 72 + i * 24, ymod, a * 72 + i * 24 + 23, ymod + 23, VARSEG(walkanim(0)), VARPTR(walkanim(walksize * (a * 3 + i)))
	b = walkframeloc(i)
	rcSrc.x = a * 72 + b * 24
	rcSrc.y = ymod
	rcSrc.w = 24
	rcSrc.h = 24

	rcDest.x = i * 24
	rcDest.y = a * 24
	rcDest.w = 24
	rcDest.h = 24

	SDL_BlitSurface temp, @rcSrc, anims(anim), @rcDest
	NEXT
  NEXT


	d = SDL_SaveBMP(anims(anim), "art\anims" + str$(anim) + ".bmp")

	t = SDL_SetColorKey(anims(anim), SDL_SRCCOLORKEY, SDL_MapRGB(video->format, 255, 0, 255))


	SDL_FreeSurface temp

end sub


sub sys_Initialize ()

	dim result as unsigned integer

	HWACCEL = 0
	HWSURFACE = SWSURFACE

	open "config.ini" for input as #1
		input #1, blank$
		input #1, SCR_WIDTH
		input #1, blank$
		input #1, SCR_HEIGHT
		input #1, blank$
		input #1, SCR_BITS
		input #1, a$
		if a$ = "HWACCEL:YES" then HWACCEL = SDL_HWACCEL
		input #1, a$
		if a$ = "HWSURFACE:YES" then HWSURFACE = SDL_HWSURFACE
		input #1, a$
		if a$ = "FULLSCREEN:YES" then opfullscreen = SDL_FULLSCREEN
		input #1, a$
		if a$ = "MUSIC:YES" then opmusic = 1
		input #1, a$
		if a$ = "SNDEFFECTS:YES" then opeffects = 1
		input #1, a$
		input #1, opmusicvol
		input #1, a$
		input #1, opeffectsvol
	close #1

	SCR_TOPX = SCR_WIDTH / 2 - 160
	SCR_TOPY = SCR_HEIGHT / 2 - 120

	fullscreen = opfullscreen OR HWACCEL OR SDL_SWSURFACE
	'fullscreen = SDL_FULLSCREEN OR HWACCEL OR SDL_HWSURFACE

	result = SDL_Init(SDL_INIT_EVERYTHING)
	if result <> 0 then
		MessageBox NULL, "Failed to init SDL", "ERROR", MB_ICONASTERISK
		end 1
	end if

	video = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, SCR_BITS, fullscreen)'SDL_FULLSCREEN
	if video = 0 then
		SDL_Quit
		MessageBox NULL, "Failed to init Video", "ERROR", MB_ICONASTERISK
		end 1
	end if

	SDL_WM_SetCaption("The Griffon Legend", null)

	t = SDL_ShowCursor (SDL_DISABLE)

	videobuffer = SDL_DisplayFormat(video)
	videobuffer2 = SDL_DisplayFormat(video)
	videobuffer3 = SDL_DisplayFormat(video)
	mapbg = SDL_CreateRGBSurface(HWSURFACE, 640, 480, SCR_BITS, video->format->rmask, video->format->gmask, video->format->bmask, video->format->amask)
	clipbg = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, SCR_BITS, video->format->rmask, video->format->gmask, video->format->bmask, video->format->amask)
	clipbg2 = SDL_DisplayFormat(video)

	for i = 0 to 3
		mapimg(i) = SDL_DisplayFormat(video)
		mapimg(i) = IMG_Load("art\map" + ltrim$(rtrim$(str$(i + 1))) + ".bmp")
		t = SDL_SetColorKey(mapimg(i), SDL_SRCCOLORKEY, SDL_MapRGB(mapimg(i)->format, 255, 0, 255))
	next

	cloudimg = IMG_Load("art\clouds.bmp")
	t = SDL_SetColorKey(cloudimg, SDL_SRCCOLORKEY, SDL_MapRGB(cloudimg->format, 255, 0, 255))
	SDL_SetAlpha cloudimg, 0 OR SDL_SRCALPHA, 96


	saveloadimg = IMG_Load("art\saveloadnew.bmp")
	t = SDL_SetColorKey(saveloadimg, SDL_SRCCOLORKEY, SDL_MapRGB(saveloadimg->format, 255, 0, 255))
	SDL_SetAlpha saveloadimg, 0 OR SDL_SRCALPHA, 160

	titleimg = IMG_Load("art\titleb.bmp")
	titleimg2 = IMG_Load("art\titlea.bmp")
	t = SDL_SetColorKey(titleimg2, SDL_SRCCOLORKEY, SDL_MapRGB(titleimg2->format, 255, 0, 255))
	'SDL_SetAlpha titleimg2, 0 OR SDL_SRCALPHA, 204

	inventoryimg = IMG_Load("art\inventory.bmp")
	t = SDL_SetColorKey(inventoryimg, SDL_SRCCOLORKEY, SDL_MapRGB(inventoryimg->format, 255, 0, 255))

	logosimg = SDL_DisplayFormat(video)
	theendimg = SDL_DisplayFormat(video)
	logosimg = IMG_Load("art\logos.bmp")
	theendimg = IMG_Load("art\theend.bmp")


	sys_loadTiles
	sys_loadTriggers
	sys_loadObjectDB
	sys_loadAnims
	sys_loadFont
	sys_loadItemImgs

	fpsr = 1
	nextticks = ticks + 1000

	maxlevel = 22

	SDL_WM_SetCaption("The Griffon Legend", null)



	for i = 0 to 15

		playerattackofs(0, i, 0) = 0'-1'-(i + 1)
		playerattackofs(0, i, 1) = -sin(3.14159 * 2 * (i + 1) / 16) * 2 -1

		playerattackofs(1, i, 0) = 0'i + 1
		playerattackofs(1, i, 1) = -sin(3.14159 * 2 * (i + 1) / 16) * 2 +1

		playerattackofs(2, i, 0) = -1'-(i + 1)
		playerattackofs(2, i, 1) = -sin(3.14159 * 2 * (i + 1) / 16) * 2

		playerattackofs(3, i, 0) = 1'i + 1
		playerattackofs(3, i, 1) = -sin(3.14159 * 2 * (i + 1) / 16) * 2

	next


	for y = 0 to 14
		for x = 0 to 19

			read elementmap(x, y)

		next
	next



	sys_setupaudio


	for i = 0 to 37
		read story$(i)
	next

	for i = 0 to 26
		read story2$(i)
	next

	for i = 0 to 3
		for y = 0 to 6
			for x = 0 to 12
				read invmap(i, x, y)
			next
		next
	next




end sub

sub sys_line (byval buffer as SDL_Surface ptr, x1, y1, x2, y2, col)

	dim temp as uinteger ptr

	d = SDL_LockSurface (buffer)

	xdif = x2 - x1
	ydif = y2 - y1

	if xdif = 0 then

		for y = y1 to y2

			temp = 	buffer->pixels + y * buffer->pitch + x1 * buffer->format->BytesPerPixel
			*temp = col
		next
	end if

	if ydif = 0 then

		for x = x1 to x2

			temp = 	buffer->pixels + y1 * buffer->pitch + x * buffer->format->BytesPerPixel
			*temp = col
		next
	end if

	SDL_UnLockSurface buffer

end sub



SUB sys_LoadAnims ()

'	sys_customLoad

	spellimg = IMG_Load("art\spells.bmp")
	t = SDL_SetColorKey(spellimg, SDL_SRCCOLORKEY, SDL_MapRGB(spellimg->format, 255, 0, 255))


	anims(0) = IMG_Load("art\anims0.bmp")
	t = SDL_SetColorKey(anims(0), SDL_SRCCOLORKEY, SDL_MapRGB(anims(0)->format, 255, 0, 255))

	animsa(0) = IMG_Load("art\anims0a.bmp")
	t = SDL_SetColorKey(animsa(0), SDL_SRCCOLORKEY, SDL_MapRGB(animsa(0)->format, 255, 0, 255))

	anims(13) = IMG_Load("art\anims0x.bmp")
	t = SDL_SetColorKey(anims(13), SDL_SRCCOLORKEY, SDL_MapRGB(anims(13)->format, 255, 0, 255))

	animsa(13) = IMG_Load("art\anims0xa.bmp")
	t = SDL_SetColorKey(animsa(13), SDL_SRCCOLORKEY, SDL_MapRGB(animsa(13)->format, 255, 0, 255))



	anims(1) = IMG_Load("art\anims1.bmp")
	t = SDL_SetColorKey(anims(1), SDL_SRCCOLORKEY, SDL_MapRGB(anims(1)->format, 255, 0, 255))

	animsa(1) = IMG_Load("art\anims1a.bmp")
	t = SDL_SetColorKey(animsa(1), SDL_SRCCOLORKEY, SDL_MapRGB(animsa(1)->format, 255, 0, 255))


	anims(2) = IMG_Load("art\anims2.bmp")
	t = SDL_SetColorKey(anims(2), SDL_SRCCOLORKEY, SDL_MapRGB(anims(2)->format, 255, 0, 255))
	'huge
	animset2(0).xofs = 8
	animset2(0).yofs = 7
	animset2(0).x = 123
	animset2(0).y = 0
	animset2(0).w = 18
	animset2(0).h = 16
	'big
	animset2(1).xofs = 7
	animset2(1).yofs = 7
	animset2(1).x = 107
	animset2(1).y = 0
	animset2(1).w = 16
	animset2(1).h = 14
	'med
	animset2(2).xofs = 6
	animset2(2).yofs = 6
	animset2(2).x = 93
	animset2(2).y = 0
	animset2(2).w = 14
	animset2(2).h = 13
	'small
	animset2(3).xofs = 4
	animset2(3).yofs = 4
	animset2(3).x = 83
	animset2(3).y = 0
	animset2(3).w = 10
	animset2(3).h = 10
	'wing
	animset2(4).xofs = 4
	animset2(4).yofs = 20
	animset2(4).x = 42
	animset2(4).y = 0
	animset2(4).w = 41
	animset2(4).h = 33
	'head
	animset2(5).xofs = 20
	animset2(5).yofs = 18
	animset2(5).x = 0
	animset2(5).y = 0
	animset2(5).w = 42
	animset2(5).h = 36

	anims(9) = IMG_Load("art\anims9.bmp")
	t = SDL_SetColorKey(anims(9), SDL_SRCCOLORKEY, SDL_MapRGB(anims(9)->format, 255, 0, 255))
	'huge
	animset9(0).xofs = 8
	animset9(0).yofs = 7
	animset9(0).x = 154
	animset9(0).y = 0
	animset9(0).w = 18
	animset9(0).h = 16
	'big
	animset9(1).xofs = 7
	animset9(1).yofs = 7
	animset9(1).x = 138
	animset9(1).y = 0
	animset9(1).w = 16
	animset9(1).h = 14
	'med
	animset9(2).xofs = 6
	animset9(2).yofs = 6
	animset9(2).x = 93 + 31
	animset9(2).y = 0
	animset9(2).w = 14
	animset9(2).h = 13
	'small
	animset9(3).xofs = 4
	animset9(3).yofs = 4
	animset9(3).x = 83 + 31
	animset9(3).y = 0
	animset9(3).w = 10
	animset9(3).h = 10
	'wing
	animset9(4).xofs = 36
	animset9(4).yofs = 20
	animset9(4).x = 42
	animset9(4).y = 0
	animset9(4).w = 72
	animset9(4).h = 33
	'head
	animset9(5).xofs = 20
	animset9(5).yofs = 18
	animset9(5).x = 0
	animset9(5).y = 0
	animset9(5).w = 42
	animset9(5).h = 36


	anims(3) = IMG_Load("art\anims3.bmp")
	t = SDL_SetColorKey(anims(3), SDL_SRCCOLORKEY, SDL_MapRGB(anims(3)->format, 255, 0, 255))

	anims(4) = IMG_Load("art\anims4.bmp")
	t = SDL_SetColorKey(anims(4), SDL_SRCCOLORKEY, SDL_MapRGB(anims(4)->format, 255, 0, 255))

	anims(5) = IMG_Load("art\anims5.bmp")
	t = SDL_SetColorKey(anims(5), SDL_SRCCOLORKEY, SDL_MapRGB(anims(5)->format, 255, 0, 255))

	anims(6) = IMG_Load("art\anims6.bmp")
	t = SDL_SetColorKey(anims(6), SDL_SRCCOLORKEY, SDL_MapRGB(anims(6)->format, 255, 0, 255))

	anims(7) = IMG_Load("art\anims7.bmp")
	t = SDL_SetColorKey(anims(7), SDL_SRCCOLORKEY, SDL_MapRGB(anims(7)->format, 255, 0, 255))

	anims(8) = IMG_Load("art\anims8.bmp")
	t = SDL_SetColorKey(anims(8), SDL_SRCCOLORKEY, SDL_MapRGB(anims(8)->format, 255, 0, 255))

	anims(10) = IMG_Load("art\anims10.bmp")
	t = SDL_SetColorKey(anims(10), SDL_SRCCOLORKEY, SDL_MapRGB(anims(10)->format, 255, 0, 255))
	animsa(10) = IMG_Load("art\anims10a.bmp")
	t = SDL_SetColorKey(animsa(10), SDL_SRCCOLORKEY, SDL_MapRGB(animsa(10)->format, 255, 0, 255))

	anims(11) = IMG_Load("art\anims11.bmp")
	t = SDL_SetColorKey(anims(11), SDL_SRCCOLORKEY, SDL_MapRGB(anims(11)->format, 255, 0, 255))
	animsa(11) = IMG_Load("art\anims11a.bmp")
	t = SDL_SetColorKey(animsa(11), SDL_SRCCOLORKEY, SDL_MapRGB(animsa(11)->format, 255, 0, 255))

	anims(12) = IMG_Load("art\anims12.bmp")
	t = SDL_SetColorKey(anims(12), SDL_SRCCOLORKEY, SDL_MapRGB(anims(12)->format, 255, 0, 255))

END SUB


sub sys_LoadItemImgs ()
	dim temp as SDL_Surface ptr


	temp = IMG_Load("art\icons.bmp")

	for i = 0 to 20
		itemimg(i) = SDL_CreateRGBSurface(SDL_SWSURFACE, 16, 16, SCR_BITS, video->format->rmask, video->format->gmask, video->format->bmask, video->format->amask)
		t = SDL_SetColorKey(itemimg(i), SDL_SRCCOLORKEY, SDL_MapRGB(itemimg(i)->format, 255, 0, 255))

		rcSrc.x = i * 16
		rcSrc.y = 0
		rcSrc.w = 16
		rcSrc.h = 16

		SDL_BlitSurface temp, @rcSrc, itemimg(i), null

	next

	SDL_FreeSurface temp


end sub


sub sys_LoadFont ()

	dim font as SDL_Surface ptr

	font = IMG_Load("art\font.bmp")

	FOR i = 32 TO 255
		for f = 0 to 4

			i2 = i - 32

			fontchr(i2, f) = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 8, SCR_BITS, video->format->rmask, video->format->gmask, video->format->bmask, video->format->amask)
			t = SDL_SetColorKey(fontchr(i2, f), SDL_SRCCOLORKEY, SDL_MapRGB(fontchr(i2, f)->format, 255, 0, 255))

			col = i2 MOD 40

			row = (i2 - Col) / 40

			rcSrc.x = col * 8
			rcSrc.y = row * 8 + f * 48
			rcSrc.w = 8
			rcSrc.h = 8


			rcDest.x = 0
			rcDest.y = 0
			SDL_BlitSurface font, @rcSrc, fontchr(i2, f), @rcDest
		next


	NEXT


	SDL_FreeSurface font
end sub

sub sys_LoadTiles ()

	tiles(0) = IMG_Load("art\tx.bmp")
	tiles(1) = IMG_Load("art\tx1.bmp")
	tiles(2) = IMG_Load("art\tx2.bmp")
	tiles(3) = IMG_Load("art\tx3.bmp")

	for i = 0 to 3
		t = SDL_SetColorKey(tiles(i), SDL_SRCCOLORKEY, SDL_MapRGB(tiles(i)->format, 255, 0, 255))
	next


	windowimg = IMG_Load("art\window.bmp")
	t = SDL_SetColorKey(windowimg, SDL_SRCCOLORKEY, SDL_MapRGB(windowimg->format, 255, 0, 255))

end sub


sub sys_loadTriggers()

	open "data\triggers.dat" for input as #1

	for i = 0 to 9999
		for a = 0 to 8
			input #1, triggers(i, a)
		next
	next


	close #1

end sub

SUB sys_LoadObjectDB ()


	'DIM SHARED objectinfo(255, 6)
				'nframes,xtiles,ytiles,speed,type,script, update?

	'DIM SHARED objecttile(255, 4, 2, 2, 1)
				'[objnum] [frame] [x] [y] [tile/layer]

	OPEN "objectdb.dat" FOR INPUT AS #1

	FOR a = 0 TO 32

		FOR b = 0 TO 5

			INPUT #1, objectinfo(a, b)
		NEXT
		FOR b = 0 TO 8
			FOR c = 0 TO 2
				FOR d = 0 TO 2
					FOR e = 0 TO 1

						INPUT #1, objecttile(a, b, c, d, e)

					NEXT
				NEXT
			NEXT
		NEXT
	NEXT

	CLOSE #1


END SUB

sub sys_print (byval buffer as SDL_Surface ptr, stri$, xloc, yloc, col)



	l = len(stri$)

	for i = 1 to l

		ac = asc(mid$(stri$, i, 1)) - 32

		rcDest.x = xloc + (i - 1) * 8
		rcDest.y = yloc

		SDL_BlitSurface fontchr(ac, col), null, buffer, @rcDest

	next


end sub

sub sys_progress (w, wm)

		dim ccc as long

		ccc = SDL_MapRGB(videobuffer->format, 0, 255, 0)

		rcDest.w = w / wm * 74
		SDL_FillRect video, @rcDest, ccc
		SDL_Flip video

end sub


sub sys_setupAudio ()

	dim loadimg as SDL_Surface ptr

	menabled = 1
	FSOUND_Init(44100, 512, 0)


	stri$ = "Loading..."
	sys_print video, stri$, SCR_TOPX + 160 - 4 * len(stri$), SCR_TOPY + 116, 0

	loadimg = IMG_Load("art\load.bmp")
	t = SDL_SetColorKey(loadimg, SDL_SRCCOLORKEY, SDL_MapRGB(loadimg->format, 255, 0, 255))

	SDL_Flip video

	rcSrc.x = 0
	rcSrc.y = 0
	rcSrc.w = 88
	rcSrc.h = 32

	rcDest.x = SCR_TOPX + 160 - 44
	rcDest.y = SCR_TOPY + 116 + 12

	SDL_SetAlpha loadimg, 0 OR SDL_SRCALPHA, 160'128
	SDL_BlitSurface loadimg, @rcSrc, video, @rcDest
	SDL_SetAlpha loadimg, 0 OR SDL_SRCALPHA, 255


	rcDest.x = SCR_TOPX + 160 - 44 + 7
	rcDest.y = SCR_TOPY + 116 + 12 + 12
	rcDest.h = 8


	mboss = FSOUND_Sample_Load(FSOUND_FREE, "music/boss.ogg", 0, 0, 0)
	IF mboss = 0 THEN
		menabled = 0
		FSOUND_Close
	End If

	if menabled = 1 then

		sys_progress 1, 21
		mgardens = FSOUND_Sample_Load(FSOUND_FREE, "music/gardens.ogg", 0, 0, 0)
		sys_progress 2, 21
		mgardens2 = FSOUND_Sample_Load(FSOUND_FREE, "music/gardens2.ogg", 0, 0, 0)
		sys_progress 3, 21
		mgardens3 = FSOUND_Sample_Load(FSOUND_FREE, "music/gardens3.ogg", 0, 0, 0)
		sys_progress 4, 21
		mgardens4 = FSOUND_Sample_Load(FSOUND_FREE, "music/gardens4.ogg", 0, 0, 0)
		sys_progress 5, 21
		mendofgame = FSOUND_Sample_Load(FSOUND_FREE, "music/endofgame.ogg", 0, 0, 0)
		sys_progress 6, 21
		mmenu = FSOUND_Sample_Load(FSOUND_FREE, "music/menu.ogg", 0, 0, 0)
		sys_progress 7, 21

		FSOUND_Sample_SetMode(mboss, FSOUND_LOOP_NORMAL)
		FSOUND_Sample_SetMode(mgardens, FSOUND_LOOP_OFF)
		FSOUND_Sample_SetMode(mgardens2, FSOUND_LOOP_OFF)
		FSOUND_Sample_SetMode(mgardens3, FSOUND_LOOP_OFF)
		FSOUND_Sample_SetMode(mgardens4, FSOUND_LOOP_OFF)
		FSOUND_Sample_SetMode(mmenu, FSOUND_LOOP_NORMAL)
		FSOUND_Sample_SetMode(mendofgame, FSOUND_LOOP_NORMAL)

		sfx(0) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/bite.ogg", 0, 0, 0)
		sys_progress 8, 21
		sfx(1) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/crystal.ogg", 0, 0, 0)
		sys_progress 9, 21
		sfx(2) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/door.ogg", 0, 0, 0)
		sys_progress 10, 21
		sfx(3) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/enemyhit.ogg", 0, 0, 0)
		sys_progress 11, 21
		sfx(4) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/ice.ogg", 0, 0, 0)
		sys_progress 12, 21
		sfx(5) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/lever.ogg", 0, 0, 0)
		sys_progress 13, 21
		sfx(6) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/lightning.ogg", 0, 0, 0)
		sys_progress 14, 21
		sfx(7) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/metalhit.ogg", 0, 0, 0)
		sys_progress 15, 21
		sfx(8) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/powerup.ogg", 0, 0, 0)
		sys_progress 16, 21
		sfx(9) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/rocks.ogg", 0, 0, 0)
		sys_progress 17, 21
		sfx(10) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/swordhit.ogg", 0, 0, 0)
		sys_progress 18, 21
		sfx(11) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/throw.ogg", 0, 0, 0)
		sys_progress 19, 21
		sfx(12) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/chest.ogg", 0, 0, 0)
		sys_progress 20, 21
		sfx(13) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/fire.ogg", 0, 0, 0)
		sys_progress 21, 21
		sfx(14) = FSOUND_Sample_Load(FSOUND_FREE, "sfx/beep.ogg", 0, 0, 0)

		for i = 0 to 14
			FSOUND_Sample_SetMode(sfx(i), FSOUND_LOOP_OFF)
		next


	end if
end sub


sub sys_update ()



	dim temp as uinteger ptr, bgc as uinteger



	'SDL_UpdateRect video, 0, 0, SCR_WIDTH, SCR_HEIGHT
	SDL_Flip video
	SDL_PumpEvents

	tickspassed = ticks
	ticks = SDL_GetTicks

	tickspassed = ticks - tickspassed
	fpsr = tickspassed / 24

	fp = fp + 1
	if ticks > nextticks then
		nextticks = ticks + 1000
		fps = fp
		fp = 0
		secsingame = secsingame + 1
	end if



	d = SDL_LockSurface (clipbg)

	if attacking = 1 then
		player.attackframe = player.attackframe + player.attackspd * fpsr
		if player.attackframe >= 16 then
			attacking = 0
			player.attackframe = 0
			player.walkframe = 0
		end if

		pa = int(player.attackframe)

		for i = 0 to pa
			if playerattackofs (player.walkdir, i, 2) = 0 then
				playerattackofs (player.walkdir, i, 2) = 1

				opx! = player.px
				opy! = player.py

				player.px = player.px + playerattackofs (player.walkdir, i, 0)
				player.py = player.py + playerattackofs (player.walkdir, i, 1)

				sx = int(player.px / 2 + 6)
				sy = int(player.py / 2 + 10)
				temp = clipbg->pixels + sy * clipbg->pitch + sx * clipbg->format->BytesPerPixel
				bgc = *temp
				if bgc > 0 then
					player.px = opx!
					player.py = opy!
				end if

			end if
		next


		player.opx = player.px
		player.opy = player.py

		game_checkhit
	end if

	for i = 0 to maxfloat

		if floattext(i, 0) > 0 then
			spd! = .5 * fpsr
			floattext(i, 0) = floattext(i, 0) - spd!
			floattext(i, 2) = floattext(i, 2) - spd!
			if floattext(i, 0) < 0 then floattext(i, 0) = 0
		end if

		if floaticon(i, 0) > 0 then
			spd! = .5 * fpsr
			floaticon(i, 0) = floaticon(i, 0) - spd!
			floaticon(i, 2) = floaticon(i, 2) - spd!
			if floaticon(i, 0) < 0 then floaticon(i, 0) = 0
		end if

	next


	if player.level = maxlevel then player.exp = 0

	if player.exp >= player.nextlevel then
		player.level = player.level + 1
		game_addFloatText "LEVEL UP!", player.px + 16 - 36, player.py + 16, 3
		player.exp = player.exp - player.nextlevel
		'player.nextlevel = player.level * 75
		player.nextlevel = player.nextlevel * 2
		player.maxhp = player.maxhp + player.level * 3
		if player.maxhp > 999 then player.maxhp = 999
		player.hp = player.maxhp

		player.sworddamage = player.level * 1.4
		player.spelldamage = player.level * 1.3

		if menabled = 1 and opeffects = 1 then
			snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndpowerup))
			FSOUND_setVolume(snd, opeffectsvol)
		end if
	end if

	SDL_UnLockSurface clipbg

	SDL_BlitSurface clipbg2, null, clipbg, null

	rc.x = player.px - 2
	rc.y = player.py - 2
	rc.w = 5
	rc.h = 5

	SDL_FillRect clipbg, @rc, 1000

	if forcepause = 0 then
		if player.foundcrystal = 1 then player.crystalcharge = player.crystalcharge + 5 * player.level * .001 * fpsr
		if player.crystalcharge > 100 then player.crystalcharge = 100

		for i = 0 to 4
			if player.foundspell(i) = 1 then player.spellcharge(i) = player.spellcharge(i) + 1 * player.level * .01 * fpsr
			if player.spellcharge(i) > 100 then player.spellcharge(i) = 100
		next

		player.attackstrength = player.attackstrength  + (30 + 3 * player.level) / 50 * fpsr

		if player.foundcrystal then
			player.spellstrength = player.spellstrength + 3 * player.level * .01 * fpsr
		end if

	end if

	if player.attackstrength > 100 then player.attackstrength = 100

	if player.spellstrength > 100 then player.spellstrength = 100

	itemyloc = itemyloc + .75 * fpsr
	while itemyloc >= 16: itemyloc = itemyloc - 16: wend


	if player.hp <= 0 then game_theend

	if roomlock = 1 then
		roomlock = 0
		for i = 1 to lastnpc
			if npcinfo(i).hp > 0 then roomlock = 1
		next
	end if


	clouddeg = clouddeg + .1 * fpsr
	while clouddeg >= 360: clouddeg = clouddeg - 360: wend

	player.hpflash = player.hpflash + .1 * fpsr
	if player.hpflash >=2 then
		player.hpflash = 0
		player.hpflashb = player.hpflashb + 1
		if player.hpflashb = 2 then player.hpflashb = 0
		if menabled = 1 and opeffects = 1 and player.hpflashb = 0 and player.hp < player.maxhp / 4 then
			snd = FSOUND_PlaySound(FSOUND_FREE, sfx(sndbeep))
			FSOUND_setVolume(snd, opeffectsvol)
		end if
	end if


	'cloudson = 0

	if itemselon = 1 then player.itemselshade = player.itemselshade + 2 * fpsr
	if player.itemselshade > 24 then player.itemselshade = 24

	for i = 0 to 4
		if inventory(i) > 9 then inventory(i) = 9
	next

end sub
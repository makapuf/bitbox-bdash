#define NB_LEVELS 9

const struct Level levels[] = {
{
	.title="intro",
	.points=10, .extra=10, .time=150, .diamonds=12,
	.msg="pick up jewels and exit before time is up",
	.map = \
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"W...... ..d.r .....r.r....... ....r....W"
	"W.rXr...... .........rd..r.... ..... ..W"
	"W.......... ..r.....r.r..r........r....W"
	"Wr.rr.........r......r..r....r...r.....W"
	"Wr. r......... r..r........r......r.rr.W"
	"W... ..r........r.....r. r........r.rr.W"
	"Wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww...r..r.W"
	"W. ...r..d. ..r.r..........d.rd...... .W"
	"W..d.....r..... ........rr r..r....r...W"
	"W...r..r.r..............r .r..r........W"
	"W.r.....r........rrr.......r.. .d....r.W"
	"W.d.. ..r.  .....r.rd..d....r...r..d. .W"
	"W. r..............r r..r........d.....rW"
	"W........wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwW"
	"W r.........r...d....r.....r...r.......W"
	"W r......... r..r........r......r.rr..PW"
	"W. ..r........r.....r.  ....d...r.rr...W"
	"W....rd..r........r......r.rd......r...W"
	"W... ..r. ..r.rr.........r.rd...... ..rW"
	"W.d.... ..... ......... .r..r....r...r.W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="rooms",
	.points=20, .extra=50, .time=150, .diamonds=10,
	.msg="pick up jewels, but you must move boulders to get all jewels",
	.map= \
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"W.r..r..w.r...d.w... .r.wr......w..rr..W"
	"W.......w......rwrr. ...w ..d...w....r.W"
	"W                                      W"
	"Wd......w.r....rw.r. .. w..r..d.w..r.r.W"
	"W.......w.r....rw.r. r..w.....r.w... ..W"
	"Wwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwwwwW"
	"W....rr.w..r....w... ..rw....r..w.....rW"
	"W.......w.. ....w... ...w....r. w.....rW"
	"W                                      W"
	"Wr..r...w....r..w..r ...w......dwr.....W"
	"Wr....r.w..r..r.w... . rw.......wr...r.W"
	"W.r.....w...r...w... . rw.......w r..r.W"
	"Wwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwwwwW"
	"Wr.  q..w....r.rw... ...w.rd..r.w......W"
	"W.....r.wr......w..d ...w ..r...w.r.rr.W"
	"W                                      W"
	"Wd.. .r.wr....r.w.r. ..rw.r.r...w......W"
	"W.....r.wr..d...w... r..w..r....w...rr W"
	"W.d... rw..r....w.Xd r..w. .....w...rr W"
	"W.r.... w.. ..r.w.P. ...w....r.rw.... .W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="Maze",
	.points=15, .extra=0, .diamonds=24, .time=150,
	.msg="pick up jewels. you must get every jewel to exit",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"Wr.ww.wrr.w...rwr..r....w...r.....rw.d.W"
	"W..Xw.d.r.w...www..w.r....r..r.r...w.wrW"
	"W....w..rd..r....w.....r.wwr.......w.wwW"
	"Wd.w..wrwr..r....w...r......r.rr......wW"
	"Wr.w...w..r.ww..r.wwd.......r.rr......wW"
	"Wrr..r....w...r......r.rr......r..dww..W"
	"W..r.ww..r.rr...w....r.rr......w..r.w.rW"
	"W..w...d......d.r..wwr..r.w.wr..wr..d.rW"
	"Wr.r....w.ww..d.r..wwr..r..d.w...w..r.wW"
	"W.r.ww.....rrwr..d.w.wr..wr...wr..d.r..W"
	"Ww.ww......rrwr..r.w.ww...w..r.ww..r.wwW"
	"W.w.r.r.w...wwr..r....w...r.....ww.r.wwW"
	"W.w.r.r.w.d.w.wr..wr....r..r.rr....w...W"
	"Ww..wrwr..r....w...d...w.rw......w.ww.dW"
	"Ww...wwr..w.d...wr..r.r...r.wr......w..W"
	"Ww.d....r.ww..r.wwr.......r.wr......w..W"
	"W..r....w...r......r.rr......w..r.w...wW"
	"Wr.ww..r.ww...w....r.rr......w..rd..r..W"
	"Ww...r......r.rd......r...ww..wr..d.w..W"
	"Wrr...w.....r.rd......w..r.wd.d.rw.r...W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="Butterflies",
	.points=5, .extra=20, .time=120, .diamonds=36,
	.msg= "drop boulders on butterflies to create jewels",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WX.....r....................r........r.W"
	"W.....r..............r.................W"
	"W........r..r..........................W"
	"Wr.....................................W"
	"W...................r..................W"
	"W.r.....................r.........r....W"
	"W..r.....r...........r..r.............rW"
	"W......r......r.....................r..W"
	"W........ B...r.. B...... B...... B....W"
	"W........  ...r..  ......  ......  .r..W"
	"W......................................W"
	"W...r..............................r...W"
	"W...r.....r............................W"
	"W......r...........r..................rW"
	"W...........r.......r..................W"
	"W..r..............r....................W"
	"W.....................r.........r......W"
	"W................................r..r..W"
	"W....r......r.rr..................r....W"
	"W...........r.rr.........r..r.r.......PW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="intermission 1",
	.points=30, .extra=0, .time=10, .diamonds=6,
	.msg="",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W         r         WWWWWWWWWWWWWWWWWWWW"
	"W  X      .         WWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W                   WWWWWWWWWWWWWWWWWWWW"
	"W         b       P WWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="guards",
	.points=50, .extra=90, .time=150, .diamonds=4,
	.msg= "the jewels are there for grapping, but they are guarded by the deadly fireflies",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"WX.....................................W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"W.......  q.....  q.....  q.....  q....W"
	"W.......   .....   .....   .....   ....W"
	"W....... d ..... d ..... d ..... d ....W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"W.......  q.....  q.....  q.....  q....W"
	"W.......   .....   .....   .....   ....W"
	"W....... d ..... d ..... d ..... d ....W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"W......................................W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="Firefly dens",
	.points=40, .extra=60, .time=150, .diamonds=4,
	.msg= "each firefly is guarding a jewel",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"Wwwwwwwwww....r.r..r........r.wwwwwwwwwW"
	"W         ...........r....r...         W"
	"W  dq     ..r..........r...r..     qd  W"
	"Wwwwwwwwww..r........r......r.wwwwwwwwwW"
	"W         ......r...r.......r.         W"
	"W  dq     ....r......r.rr.....     qd  W"
	"Wwwwwwwwww.rr........r.rr.....wwwwwwwwwW"
	"W         ....r.r....r..r.....         W"
	"W  dq     ....r.r....r..r..r..     qd  W"
	"Wwwwwwwwww.rr.r..r....r...r...wwwwwwwwwW"
	"W         .rr.r..r............         W"
	"W  dq     ....r..r........r...     qd  W"
	"Wwwwwwwwww.....r...r....r..r..wwwwwwwwwW"
	"W....r.r..r........r.....r............rW"
	"W......r....r....r..r.r...r..r.........W"
	"W..r....r.....r...r.......r..r.........W"
	"W..r........r......r.rr.........r......W"
	"Wr.X...r...........r.rr.........rr..r.PW"
	"W....r......r.rr......r........r..r....W"
	"Wrr.........r.rr.........r..r.r.r..r...W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="Amoeba",
	.points=10, .extra=20, .time=120, .diamonds=15,
	.msg= "surround the amoeba with boulders, so it can't grow anymore. pick up jewels that are created when it suffocates",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"W. .. .rr..... ..r. X.... rr r..r. .  .W"
	"W ..r. .. .  .... .r.r. ...  r..r.d.. .W"
	"Wr.....  .q.  ... .r.r. ... wwwwwwwwwwwW"
	"W.r.d... .  ...... ..rr..r.... . ... . W"
	"Wwwwwwwwwwwww.r. ..   r.. .... ...r....W"
	"Wr. r...... ..r. ... ..r.  ..r.  q.....W"
	"Wr. r...... .. r..r.... ...r......r.rr.W"
	"W... ..r  ... ..r.  ..r.  ... ....r.rr.W"
	"W... ..r. .r.... ...q......r.r..  r..r.W"
	"W  .. r.... ..r.r.... .  .......  d.. .W"
	"W. ... .. .  .. .  .....rr r..r. . r.. W"
	"W.. d..r.r.... .  ......r  r..r. .  ...W"
	"W.r.  ..r.  ... .r.r. ...  r.. .... ...W"
	"W....  .r.  ... .r.r. .r. . r.. r.... .W"
	"W.  .... ....  .. r r..r.... ...r... .rW"
	"W..... .  .rr. ...  r.. .r... r..r.r...W"
	"W r...... ..r. .r.... .  ..r.  r.......W"
	"W r...... .. r..r.... ...r......r.rr...W"
	"W. ..r. ... ..r.  .aa.  ... ....r.rr...W"
	"W. .drq..r.... ...r......r.rq.....dr...W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
},{
	.title="Enchanted wall",
	.points=10, .extra=20, .time=120, .diamonds=10,
	.msg= "activate the enchanted wall and create as many jewels as you can",
	.map=\
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
	"W . r.. . .. ..r. ..X ..r.  ..r. r... .W"
	"W.r.rr...... ..r...r.... ...r.....dr.r.W"
	"W r..r...  ...r..r. ..r.r...wwwwwwwwwwwW"
	"W...d ..r. q.....r..... ........rr r..rW"
	"Wwwwwwwwwwwww..r.r.... .  ......r  r..rW"
	"W.  ... ..r.  ..r.  .... rrr.....  r.. W"
	"W... r... q.. ..r.  .....r.rr..r. . r..W"
	"W..r. ..r. r.... ..... ...r r..r.... ..W"
	"W.....r ...... .  qrr. ...  r.. .r....rW"
	"Wr.r... . r...... ..r...r....r....dr.  W"
	"W......r. r......... r..r...wwwwwwwwwwwW"
	"W.rr...... ..r. ... ..r.  ..r.  ... r..W"
	"Wwwwwwwwwwwwwr........ ...r......r.rr..W"
	"W..r...  ...d..r. ..r.rr.........r.rr..W"
	"W.. ..r. .r...mmmmmmmm.........  r..r..W"
	"Wr.. r....r..r r...d .. .......  r..r..W"
	"W ... ..r. ...r.  .....rrrr..r. . r.. rW"
	"W. r..q.r.... .  ......rr r..r...  ...rW"
	"Wr.  ..r.  .....r.r. ...  r..r.... ...rW"
	"W...  .r.r .....r.r.....   .. .r....r..W"
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
}
};
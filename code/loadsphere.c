/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:36  trq
 * Initial revision
 *
 * Revision 1.3  94/04/20  09:00:53  trq
 * Check for no particles in the box.
 * 
 * Revision 1.2  94/03/14  15:12:51  trq
 * Check for bad malloc returns.
 * 
 */

#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void add_const_mult_vec();
void cross_product();

void
loadsphere(box,center,radius)
    int box ;
    Real center[MAXDIM] ;
    Real radius ;
{ 
    int i,j ;
    double d ;
    Real ang_mom[MAXDIM] ;
    double distance() ;
    struct gas_particle *gp, *lastgp ;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;

    lastgp = gas_particles + header.nsph ;
    lastdp = dark_particles + header.ndark ;
    lastsp = star_particles + header.nstar ;
    if(boxlist[box].gpi != NULL) free(boxlist[box].gpi);
    if(boxlist[box].gp != NULL) free(boxlist[box].gp);
    if(header.nsph != 0) {
	boxlist[box].gpi =
	  (int *)malloc(header.nsph*sizeof(*boxlist[box].gpi));
	if(boxlist[box].gpi == NULL) {
	    printf("<sorry, no memory for gas particles in box, %s>\n",title) ;
	    return ;
	}
	boxlist[box].gp = (struct gas_particle **)
			    malloc(header.nsph*sizeof(*boxlist[box].gp));
	if(boxlist[box].gp == NULL) {
	    printf("<sorry, no memory for gas particles in box, %s>\n",title) ;
	    return ;
	}
    }
    else {
	boxlist[box].gpi = NULL;
	boxlist[box].gp = NULL;
    }
    
    if(boxlist[box].dpi != NULL) free(boxlist[box].dpi);
    if(boxlist[box].dp != NULL) free(boxlist[box].dp);
    if(header.ndark != 0) {
	boxlist[box].dpi = (int *)
	                     malloc(header.ndark*sizeof(*boxlist[box].dpi));
	if(boxlist[box].dpi == NULL) {
	    printf("<sorry, no memory for dark particles in box, %s>\n",title) ;
	    return ;
	}
	boxlist[box].dp = (struct dark_particle **)
			    malloc(header.ndark*sizeof(*boxlist[box].dp));
	if(boxlist[box].dp == NULL) {
	    printf("<sorry, no memory for dark particles in box, %s>\n",title) ;
	    return ;
	}
    }
    else {
	boxlist[box].dpi = NULL;
	boxlist[box].dp = NULL;
    }
    if(boxlist[box].spi != NULL) free(boxlist[box].spi);
    if(boxlist[box].sp != NULL) free(boxlist[box].sp);
    if(header.nstar != 0) {
	boxlist[box].spi =
	  (int *)malloc(header.nstar*sizeof(*boxlist[box].spi));
	if(boxlist[box].spi == NULL) {
	    printf("<sorry, no memory for star particles in box, %s>\n",title) ;
	    return ;
	}
	boxlist[box].sp = (struct star_particle **)
			    malloc(header.nstar*sizeof(*boxlist[box].sp));
	if(boxlist[box].sp == NULL) {
	    printf("<sorry, no memory for star particles in box, %s>\n",title) ;
	    return ;
	}
    }
    else {
	boxlist[box].spi = NULL;
	boxlist[box].sp = NULL;
    }
    if(box == 0) {

	boxlist[box].ngas = 0 ;
	for (gp = gas_particles, j = 0 ;gp < lastgp ;gp++, j++) {
	    d=distance(center,gp->pos) ;
	    if (d <= radius) {
		boxlist[box].gpi[boxlist[box].ngas] = j ;
		boxlist[box].gp[boxlist[box].ngas++] = gp ;
	    }
	}

	boxlist[box].ndark = 0 ;
	for (dp = dark_particles, j = 0 ;dp < lastdp ;dp++, j++) {
	    d=distance(center,dp->pos) ;
	    if (d <= radius) {
		boxlist[box].dpi[boxlist[box].ndark] = j ;
		boxlist[box].dp[boxlist[box].ndark++] = dp ;
	    }
	}

	boxlist[box].nstar = 0 ;
	for (sp = star_particles, j = 0 ;sp < lastsp ;sp++, j++) {
	    d=distance(center,sp->pos) ;
	    if (d <= radius) {
		boxlist[box].spi[boxlist[box].nstar] = j ;
		boxlist[box].sp[boxlist[box].nstar++] = sp ;
	    }
	}
    }
    else {
	boxlist[box].ngas = 0 ;
	for (i = 0 ;i < boxlist[0].ngas ;i++) {
	    gp = boxlist[0].gp[i] ;
	    d=distance(center,gp->pos) ;
	    if (d <= radius) {
		boxlist[box].gp[boxlist[box].ngas] = gp ;
		boxlist[box].gpi[boxlist[box].ngas++] = boxlist[0].gpi[i] ;
	    }
	}
	boxlist[box].ndark = 0 ;
	for (i = 0 ;i < boxlist[0].ndark ;i++) {
	    dp = boxlist[0].dp[i] ;
	    d=distance(center,dp->pos) ;
	    if (d <= radius) {
		boxlist[box].dp[boxlist[box].ndark] = dp ;
		boxlist[box].dpi[boxlist[box].ndark++] = boxlist[0].dpi[i] ;
	    }
	}
	boxlist[box].nstar = 0 ;
	for (i = 0 ;i < boxlist[0].nstar ;i++) {
	    sp = boxlist[0].sp[i] ;
	    d=distance(center,sp->pos) ;
	    if (d <= radius) {
		boxlist[box].sp[boxlist[box].nstar] = sp ;
		boxlist[box].spi[boxlist[box].nstar++] = boxlist[0].spi[i] ;
	    }
	}
    }
    if(boxlist[box].gpi) {
	boxlist[box].gpi = (int *)realloc(boxlist[box].gpi,
			    boxlist[box].ngas*sizeof(*boxlist[box].gpi));
	boxlist[box].gp = (struct gas_particle **)realloc(boxlist[box].gp,
			    boxlist[box].ngas*sizeof(*boxlist[box].gp));
    }
    if(boxlist[box].dpi) {
	boxlist[box].dpi = (int *)realloc(boxlist[box].dpi,
			    boxlist[box].ndark*sizeof(*boxlist[box].dpi));
	boxlist[box].dp = (struct dark_particle **)realloc(boxlist[box].dp,
			    boxlist[box].ndark*sizeof(*boxlist[box].dp));
    }
    if(boxlist[box].spi) {
	boxlist[box].spi = (int *)realloc(boxlist[box].spi,
			    boxlist[box].nstar*sizeof(*boxlist[box].spi));
	boxlist[box].sp = (struct star_particle **)realloc(boxlist[box].sp,
			    boxlist[box].nstar*sizeof(*boxlist[box].sp));
    }
    boxes_loaded[box] = LOADED ;

    boxes[box].size = 2.*radius ;
    boxes[box].volume = 4.*PI/3.*radius*radius*radius ;
    setvec(boxes[box].center,center) ;
    boxes[box].gas_mass = boxes[box].dark_mass = boxes[box].star_mass =
	    boxes[box].total_mass = 0.0 ;
    for (i = 0 ; i < header.ndim ; i++) {
	boxes[box].gas_com[i] = boxes[box].dark_com[i] =
		boxes[box].star_com[i] = boxes[box].total_com[i] = 0.0 ;
	boxes[box].gas_com_vel[i] = boxes[box].dark_com_vel[i] =
		boxes[box].star_com_vel[i] = boxes[box].total_com_vel[i] = 0.0 ;
	boxes[box].gas_angular_mom[i] = boxes[box].dark_angular_mom[i] =
		boxes[box].star_angular_mom[i] =
		boxes[box].total_angular_mom[i] = 0.0 ;
    }
    for (i = 0 ;i < boxlist[box].ngas ;i++) {
	gp = boxlist[box].gp[i] ;
	boxes[box].gas_mass += gp->mass ;
	add_const_mult_vec(boxes[box].gas_com, gp->mass, gp->pos) ;
	add_const_mult_vec(boxes[box].gas_com_vel, gp->mass, gp->vel) ;
	cross_product(ang_mom, gp->pos, gp->vel) ;
	add_const_mult_vec(boxes[box].gas_angular_mom, gp->mass, ang_mom) ;
    }
    for (i = 0 ;i < boxlist[box].ndark ;i++) {
	dp = boxlist[box].dp[i] ;
	boxes[box].dark_mass += dp->mass ;
	add_const_mult_vec(boxes[box].dark_com, dp->mass, dp->pos) ;
	add_const_mult_vec(boxes[box].dark_com_vel, dp->mass, dp->vel) ;
	cross_product(ang_mom, dp->pos, dp->vel) ;
	add_const_mult_vec(boxes[box].dark_angular_mom, dp->mass, ang_mom) ;
    }
    for (i = 0 ;i < boxlist[box].nstar ;i++) {
	sp = boxlist[box].sp[i] ;
	boxes[box].star_mass += sp->mass ;
	add_const_mult_vec(boxes[box].star_com, sp->mass, sp->pos) ;
	add_const_mult_vec(boxes[box].star_com_vel, sp->mass, sp->vel) ;
	cross_product(ang_mom, sp->pos, sp->vel) ;
	add_const_mult_vec(boxes[box].star_angular_mom, sp->mass, ang_mom) ;
    }
    boxes[box].total_mass = boxes[box].gas_mass + boxes[box].dark_mass +
	    boxes[box].star_mass ;
    for (i = 0 ; i < header.ndim ; i++) {
	boxes[box].total_com[i] = boxes[box].gas_com[i] +
		boxes[box].dark_com[i] + boxes[box].star_com[i] ;
	boxes[box].total_com_vel[i] = boxes[box].gas_com_vel[i] +
		boxes[box].dark_com_vel[i] + boxes[box].star_com_vel[i] ;
	boxes[box].total_angular_mom[i] = boxes[box].gas_angular_mom[i] +
		boxes[box].dark_angular_mom[i] +
		boxes[box].star_angular_mom[i] ;
	if(boxlist[box].ngas)
	  boxes[box].gas_com[i] /= boxes[box].gas_mass ;

	if(boxlist[box].ndark)
	  boxes[box].dark_com[i] /= boxes[box].dark_mass ;

	if(boxlist[box].nstar)
	  boxes[box].star_com[i] /= boxes[box].star_mass ;

	if(boxlist[box].ngas || boxlist[box].ndark || boxlist[box].nstar)
	  boxes[box].total_com[i] /= boxes[box].total_mass ;

	if(boxlist[box].ngas)
	  boxes[box].gas_com_vel[i] /= boxes[box].gas_mass ;

	if(boxlist[box].ndark)
	  boxes[box].dark_com_vel[i] /= boxes[box].dark_mass ;

	if(boxlist[box].nstar)
	  boxes[box].star_com_vel[i] /= boxes[box].star_mass ;

	if(boxlist[box].ngas || boxlist[box].ndark || boxlist[box].nstar)
	  boxes[box].total_com_vel[i] /= boxes[box].total_mass ;
    }
    cross_product(ang_mom, boxes[box].gas_com, boxes[box].gas_com_vel) ;
    add_const_mult_vec(boxes[box].gas_angular_mom,
	    -boxes[box].gas_mass, ang_mom) ;
    cross_product(ang_mom, boxes[box].dark_com, boxes[box].dark_com_vel) ;
    add_const_mult_vec(boxes[box].dark_angular_mom,
	    -boxes[box].dark_mass, ang_mom) ;
    cross_product(ang_mom, boxes[box].star_com, boxes[box].star_com_vel) ;
    add_const_mult_vec(boxes[box].star_angular_mom,
	    -boxes[box].star_mass, ang_mom) ;
    cross_product(ang_mom, boxes[box].total_com, boxes[box].total_com_vel) ;
    add_const_mult_vec(boxes[box].total_angular_mom,
	    -boxes[box].total_mass, ang_mom) ;
    for (i = 0 ; i < header.ndim ; i++) {
	if(boxlist[box].ngas)
	  boxes[box].gas_angular_mom[i] /= boxes[box].gas_mass ;

	if(boxlist[box].ndark)
	  boxes[box].dark_angular_mom[i] /= boxes[box].dark_mass ;

	if(boxlist[box].nstar)
	  boxes[box].star_angular_mom[i] /= boxes[box].star_mass ;

	if(boxlist[box].ngas || boxlist[box].ndark || boxlist[box].nstar)
	  boxes[box].total_angular_mom[i] /= boxes[box].total_mass ;
    }
}
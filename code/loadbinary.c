#include "defs.h"
#include <stdlib.h>

static double currtime = 0.0;
static long currpos = 0L ;
static long lastpos = 0L ;

void
loadbinary(infile,time)
    FILE *infile;
    double time;
{
    int old_nstar;
    int i;

    if ((float)currtime > (float)time){
	fseek(infile,0L,0);
	currtime=0.0;
	currpos=0;
    }
    old_nstar = header.nstar;
    forever {
	if(fread((char *)&header,sizeof(header),1,infile) 
	   != 1) {
	    printf("<sorry time too large %s, using %f>\n",title,
			     (float)currtime) ;
	    break ;
	}
	currtime = header.time ;
	currpos = ftell(infile) - sizeof(header);
	if ( (float)header.time >= (float)time ) 
	    break ;
	fseek(infile,
	    sizeof(gas_particles[0])*header.nsph +
	    sizeof(dark_particles[0])*header.ndark +
	    sizeof(star_particles[0])*header.nstar,
	    1) ;
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    fread((char *)&header,sizeof(header),1,infile) ;

    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
	gas_particles = (struct gas_particle *)
			    malloc(header.nsph*sizeof(*gas_particles));
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
	dark_particles = (struct dark_particle *)
			    malloc(header.ndark*sizeof(*dark_particles));
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
	star_particles = (struct star_particle *)
			    malloc(header.nstar*sizeof(*star_particles));
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      star_particles = NULL;

    if(mark_gas == NULL && header.nsph != 0)
    	mark_gas = (short *)calloc(header.nsph, sizeof(*mark_gas));
    if(mark_gas == NULL && header.nsph != 0) {
	printf("<sorry, no memory for gas particle markers, %s>\n",title) ;
	return ;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return ;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
	mark_star = (short *)realloc(mark_star,
		header.nstar*sizeof(*mark_star));
	for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return ;
    }

    if(box0_smx) {
	kdFinish(box0_smx->kd);
	smFinish(box0_smx);
	box0_smx = NULL;
    }

    fread((char *)gas_particles,sizeof(struct gas_particle),
		     header.nsph,infile) ;
    fread((char *)dark_particles,sizeof(struct dark_particle),
		     header.ndark,infile) ;
    fread((char *)star_particles,sizeof(struct star_particle),
		     header.nstar,infile) ;
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
}

void
loadbin_box(infile,time, xmin, xmax)
    FILE *infile;
    double time;
    Real *xmin;
    Real *xmax;
{
    int old_nstar;
    int i;
    int k;
    int in;
    int max_gas = 1000;
    int max_dark = 1000;
    int max_star = 1000;
    struct gas_particle *gp;
    struct dark_particle *dp;
    struct star_particle *sp;
    int ngas, ndark, nstar;

    if ((float)currtime > (float)time){
	fseek(infile,0L,0);
	currtime=0.0;
	currpos=0;
    }
    old_nstar = header.nstar;
    forever {
	if(fread((char *)&header,sizeof(header),1,infile) 
	   != 1) {
	    printf("<sorry time too large %s, using %f>\n",title,
			     (float)currtime) ;
	    break ;
	}
	currtime = header.time ;
	currpos = ftell(infile) - sizeof(header);
	if ( (float)header.time >= (float)time ) 
	    break ;
	fseek(infile,
	    sizeof(gas_particles[0])*header.nsph +
	    sizeof(dark_particles[0])*header.ndark +
	    sizeof(star_particles[0])*header.nstar,
	    1) ;
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    fread((char *)&header,sizeof(header),1,infile) ;

    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
	gas_particles = (struct gas_particle *)
			    malloc(max_gas*sizeof(*gas_particles));
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
	dark_particles = (struct dark_particle *)
			    malloc(max_dark*sizeof(*dark_particles));
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
	star_particles = (struct star_particle *)
			    malloc(max_star*sizeof(*star_particles));
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      star_particles = NULL;

    if(box0_smx) {
	kdFinish(box0_smx->kd);
	smFinish(box0_smx);
	box0_smx = NULL;
    }

    for(i = 0, gp = gas_particles; i < header.nsph; i++) {
	fread((char *)gp, sizeof(struct gas_particle),
	      1,infile) ;
	in = 1;
	for(k = 0; k < header.ndim; k++) {
	    if(gp->pos[k] < xmin[k] || gp->pos[k] >= xmax[k])
		in = 0;
	}
	if(in) {
	    ++gp;
	    if(gp - gas_particles >= max_gas) {
		ngas = gp - gas_particles;
		max_gas *= 1.4;
		gas_particles = realloc(gas_particles,
					max_gas*sizeof(*gas_particles));
		if(gas_particles == NULL) {
		    printf("<sorry, no memory for gas particles, %s>\n",title);
		    return ;
		}
		gp = gas_particles + ngas;
	    }
	}
    }
    header.nsph = gp - gas_particles;

    for(i = 0, dp = dark_particles; i < header.ndark; i++) {
	fread((char *)dp, sizeof(struct dark_particle),
	      1,infile) ;
	in = 1;
	for(k = 0; k < header.ndim; k++) {
	    if(dp->pos[k] < xmin[k] || dp->pos[k] >= xmax[k])
		in = 0;
	}
	if(in) {
	    ++dp;
	    if(dp - dark_particles >= max_dark) {
		ndark = dp - dark_particles;
		max_dark *= 1.4;
		dark_particles = realloc(dark_particles,
					 max_dark*sizeof(*dark_particles));
		if(dark_particles == NULL) {
		    printf("<sorry, no memory for dark particles, %s>\n",title);
		    return ;
		}
		dp = dark_particles + ndark;
	    }
	}
    }
    header.ndark = dp - dark_particles;

    for(i = 0, sp = star_particles; i < header.nstar; i++) {
	fread((char *)sp, sizeof(struct star_particle),
	      1,infile) ;
	in = 1;
	for(k = 0; k < header.ndim; k++) {
	    if(sp->pos[k] < xmin[k] || sp->pos[k] >= xmax[k])
		in = 0;
	}
	if(in) {
	    ++sp;
	    if(sp - star_particles >= max_star) {
		nstar = sp - star_particles;
		max_star *= 1.4;
		star_particles = realloc(star_particles,
					 max_star*sizeof(*star_particles));
		if(star_particles == NULL) {
		    printf("<sorry, no memory for star particles, %s>\n",title);
		    return ;
		}
		sp = star_particles + nstar;
	    }
	}
    }
    header.nstar = sp - star_particles;

    header.nbodies = header.nsph + header.ndark + header.nstar;

    if(mark_gas == NULL && header.nsph != 0)
    	mark_gas = (short *)calloc(header.nsph, sizeof(*mark_gas));
    if(mark_gas == NULL && header.nsph != 0) {
	printf("<sorry, no memory for gas particle markers, %s>\n",title) ;
	return ;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return ;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
	mark_star = (short *)realloc(mark_star,
		header.nstar*sizeof(*mark_star));
	for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return ;
    }
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
}

	judge[i] = cos(position[i].y()*pi)/2;
	if(position[i].x()<=judge[i]) {
	  r[i] = position[i].x()-(1/tan(sqrt(pi/2))*position[i].y());
	  if(r[i]<=-1/3) {
	    variable[i] = -r[i]*sin(3*pi/2*r[i]*r[i]);
	  }
	  else if(r[i]>-1/3 && r[i]<1/3) {
	    variable[i] = sin(2*pi*r[i]);
	  }
	  else {
	    variable[i] = 2*r[i]-1+(sin(3*pi*r[i]))/6;
	  }
	}
	else {
	  r[i] = position[i].x()+(1/tan(sqrt(pi/2))*position[i].y());
	  if(r[i]<=-1/3) {
	    variable[i] = -r[i]*sin(3*pi/2*r[i]*r[i])+cos(2*pi*position[i].y());
	  }
	  else if(r[i]>-1/3 && r[i]<1/3) {
	    variable[i] = sin(2*pi*r[i])+cos(2*pi*position[i].y());
	  }
	  else {
	    variable[i] = 2*r[i]-1+(sin(3*pi*r[i]))/6+cos(2*pi*position[i].y());
	  }
	}

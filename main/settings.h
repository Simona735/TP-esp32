
// nastavenia


struct DeviceConfigs{
	int iUltraCheckIntervalMS;					//doba spanku medzi kontrolou posty
	int iUltraExtraChecks;						//kolko krat sa pri detekcii posty kontroluje navyse na zabranenie falosnych poplachov
	int iUltraExtraChecksIntervalMS;			//cas medzi extra kontrolami
	int iUltraTrigPin = 13;
  int iUltraEchoPin = 15;
  
	
	
	
	
	
};

extern DeviceConfigs TPCFG;

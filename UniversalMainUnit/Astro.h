bool atTargetAxis1(bool update = false, double TrackingRate = 1.);
bool atTargetAxis2(bool update = false, double TrackingRate = 1.);
void ApplyTrackingRate(void);
void computeTrackingRate(bool apply);
double SetRates(double maxslewrate);
void SetAcceleration(void);
void enableGuideRate(int g);
void initMaxSpeed(void);
bool isAltAz(void);

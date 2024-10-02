#ifndef PAYOFF_H
#define PAYOFF_H

class PayOff {
  public:
    PayOff();
    virtual ~PayOff();
};

class PayOffFixedStrike : public PayOff {
  public:
    PayOffFixedStrike(double strike);
    virtual ~PayOffFixedStrike();

    virtual double call(const double stat) const = 0;
    virtual double put(const double stat) const = 0;

  protected:
    double strike;
};

class PayOffFloatingStrike : public PayOff {
  public:
    PayOffFloatingStrike();
    virtual ~PayOffFloatingStrike();

    virtual double call(const double stat, const double s) const = 0;
    virtual double put(const double stat, const double s) const = 0;
};

class AsianFixedStrikePayOff : public PayOffFixedStrike {
  public:
    AsianFixedStrikePayOff(double strike);
    ~AsianFixedStrikePayOff();

    double call(const double avg) const override;
    double put(const double avg) const override;
};

class AsianFloatingStrikePayOff : public PayOffFloatingStrike {
  public:
    AsianFloatingStrikePayOff();
    ~AsianFloatingStrikePayOff();

    double call(const double avg, const double s) const override;
    double put(const double avg, const double s) const override;
};

class LookbackFixedStrikePayOff : public PayOffFixedStrike {
  public:
    LookbackFixedStrikePayOff(double strike);
    ~LookbackFixedStrikePayOff();

    double call(const double max) const override;
    double put(const double min) const override;
};

class LookbackFloatingStrikePayOff : public PayOffFloatingStrike {
  public:
    LookbackFloatingStrikePayOff();
    ~LookbackFloatingStrikePayOff();

    double call(const double min, const double s) const override;
    double put(const double max, const double s) const override;
};

#endif
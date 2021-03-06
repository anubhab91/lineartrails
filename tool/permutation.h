#ifndef PERMUTATION_H_
#define PERMUTATION_H_

#include <vector>
#include <memory>

#include "layer.h"
#include "statemask.h"


struct Permutation {
  typedef std::unique_ptr<Permutation> PermPtr;
  Permutation(unsigned int rounds);
  Permutation(const Permutation& other);
  virtual ~Permutation(){};
  virtual bool checkchar(std::ostream& stream = std::cout);
  virtual bool update();
  virtual void print(std::ostream& stream);
  virtual PermPtr clone() const = 0;
  virtual void set(Permutation* perm);
  virtual void save();
  virtual void restore();
  virtual void SboxStatus(std::vector<SboxPos>& active, std::vector<SboxPos>& inactive);
  virtual void SboxStatus(std::vector<std::vector<SboxPos>>& active, std::vector<std::vector<SboxPos>>& inactive);
  virtual bool isActive(SboxPos pos);
  virtual bool guessbestsbox(SboxPos pos, std::function<int(int, int, int)> rating);
  virtual bool guessbestsboxrandom(SboxPos pos, std::function<int(int, int, int)> rating, int num_alternatives);
  virtual bool guessbestsbox(SboxPos pos, std::function<int(int, int, int)> rating, int num_alternatives);
  virtual void PrintWithProbability(std::ostream& stream = std::cout, unsigned int offset = 0);
  virtual void touchall();
  virtual double GetProbability();
  virtual unsigned int GetActiveSboxes();
  virtual bool setBit(BitMask cond, unsigned int bit);
  bool setBit(const char cond, unsigned int bit, unsigned char num_words, unsigned char num_bits);
  virtual bool setBox(bool active, unsigned int box_num);

  std::vector<std::unique_ptr<StateMaskBase>> state_masks_;
  std::vector<std::unique_ptr<SboxLayerBase>> sbox_layers_;
  std::vector<std::unique_ptr<LinearLayer>> linear_layers_;
  bool toupdate_linear;
  bool toupdate_nonlinear;
  unsigned int rounds_;

  std::vector<std::unique_ptr<StateMaskBase>> saved_state_masks_;
  std::vector<std::unique_ptr<SboxLayerBase>> saved_sbox_layers_;
  std::vector<std::unique_ptr<LinearLayer>> saved_linear_layers_;
  bool saved_toupdate_linear;
  bool saved_toupdate_nonlinear;
};


#endif // PERMUTATION_H_

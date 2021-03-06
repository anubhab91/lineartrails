#include "hamsi_permutation.h"


HamsiPermutation::HamsiPermutation(unsigned int rounds) : Permutation(rounds) {
  for(unsigned int i = 0; i< 2*rounds +1; ++i){
      this->state_masks_[i].reset(new HamsiState);
      this->saved_state_masks_[i].reset(new HamsiState);
    }
  for (unsigned int i = 0; i < rounds; ++i) {
    this->sbox_layers_[i].reset(new HamsiSboxLayer);
    this->sbox_layers_[i]->SetMasks(this->state_masks_[2*i].get(), this->state_masks_[2*i + 1].get());
    this->linear_layers_[i].reset(new HamsiLinearLayer);
    this->linear_layers_[i]->SetMasks(this->state_masks_[2*i + 1].get(), this->state_masks_[2*i + 2].get());
    this->saved_sbox_layers_[i].reset(new HamsiSboxLayer);
    this->saved_sbox_layers_[i]->SetMasks(this->saved_state_masks_[2*i].get(), this->saved_state_masks_[2*i + 1].get());
    this->saved_linear_layers_[i].reset(new HamsiLinearLayer);
    this->saved_linear_layers_[i]->SetMasks(this->saved_state_masks_[2*i + 1].get(), this->saved_state_masks_[2*i + 2].get());
  }
  touchall();
}


HamsiPermutation::HamsiPermutation(const HamsiPermutation& other) : Permutation(other) {
  for(unsigned int i = 0; i< 2*rounds_ +1; ++i){
      this->saved_state_masks_[i].reset(new HamsiState);
    }
  for (unsigned int i = 0; i < rounds_; ++i) {
    this->saved_sbox_layers_[i].reset(new HamsiSboxLayer);
    this->saved_sbox_layers_[i]->SetMasks(this->saved_state_masks_[2*i].get(), this->saved_state_masks_[2*i + 1].get());
    this->saved_linear_layers_[i].reset(new HamsiLinearLayer);
    this->saved_linear_layers_[i]->SetMasks(this->saved_state_masks_[2*i + 1].get(), this->saved_state_masks_[2*i + 2].get());
  }
}


HamsiPermutation& HamsiPermutation::operator=(const HamsiPermutation& rhs){
  for(unsigned int i = 0; i< 2*rounds_ +1; ++i){
    this->state_masks_[i].reset(rhs.state_masks_[i]->clone());
  }
  this->toupdate_linear = rhs.toupdate_linear;
 this->toupdate_nonlinear = rhs.toupdate_nonlinear;
 for (unsigned int i = 0; i < rounds_; ++i) {
   this->sbox_layers_[i].reset(rhs.sbox_layers_[i]->clone());
   this->sbox_layers_[i]->SetMasks(this->state_masks_[2*i].get(), this->state_masks_[2*i + 1].get());
   this->linear_layers_[i].reset(rhs.linear_layers_[i]->clone());
   this->linear_layers_[i]->SetMasks(this->state_masks_[2*i + 1].get(), this->state_masks_[2*i + 2].get());
 }
 return *this;
}


Permutation::PermPtr HamsiPermutation::clone() const {
  return PermPtr(new HamsiPermutation(*this));
}

void HamsiPermutation::PrintWithProbability(std::ostream& stream, int offset) {
  Permutation::PrintWithProbability(stream, 0);
}


void HamsiPermutation::touchall() {
  Permutation::touchall();
}

//-----------------------------------------------------------------------------

HamsiCompression::HamsiCompression(unsigned int rounds) : Permutation(rounds) {

  for(unsigned int i = 1; i< 2*rounds +1; ++i){
      this->state_masks_[i].reset(new HamsiState);
    }
  this->state_masks_[0].reset(new HamsiStateFeedForward((HamsiState*)this->state_masks_[2*rounds].get()));

  for (unsigned int i = 0; i < rounds; ++i) {

    this->sbox_layers_[i].reset(new HamsiSboxLayer);
    this->sbox_layers_[i]->SetMasks(this->state_masks_[2*i].get(), this->state_masks_[2*i + 1].get());

    this->linear_layers_[i].reset(new HamsiLinearLayer);
    this->linear_layers_[i]->SetMasks(this->state_masks_[2*i + 1].get(), this->state_masks_[2*i + 2].get());
  }

  touchall();
}


HamsiCompression::HamsiCompression(const HamsiCompression& other) : Permutation(other.rounds_) {
  for (unsigned int i = 1; i < 2 * rounds_ + 1; ++i) {
    this->state_masks_[i].reset(other.state_masks_[i]->clone());
  }

  HamsiStateFeedForward* state = (HamsiStateFeedForward*) other.state_masks_[0].get();
  this->state_masks_[0].reset(state->clone((HamsiState*) this->state_masks_[2 * rounds_].get()));

  for (unsigned int i = 0; i < rounds_; ++i) {
    this->sbox_layers_[i].reset(other.sbox_layers_[i]->clone());
    this->sbox_layers_[i]->SetMasks(this->state_masks_[2 * i].get(),
                                    this->state_masks_[2 * i + 1].get());
    this->linear_layers_[i].reset(other.linear_layers_[i]->clone());
    this->linear_layers_[i]->SetMasks(this->state_masks_[2 * i + 1].get(),
                                      this->state_masks_[2 * i + 2].get());
  }
  this->toupdate_linear = other.toupdate_linear;
  this->toupdate_nonlinear = other.toupdate_nonlinear;
}

Permutation::PermPtr HamsiCompression::clone() const {
  return PermPtr(new HamsiCompression(*this));
}

void HamsiCompression::PrintWithProbability(std::ostream& stream, int offset) {
  Permutation::PrintWithProbability(stream, 0);
}


void HamsiCompression::touchall() {
  Permutation::touchall();
}


void HamsiCompression::set(Permutation* perm){
  for(unsigned int i = 1; i< 2*rounds_ +1; ++i){
    this->state_masks_[i].reset(perm->state_masks_[i]->clone());
  }

  HamsiStateFeedForward* state = (HamsiStateFeedForward*) perm->state_masks_[0].get();
  this->state_masks_[0].reset(state->clone((HamsiState*) this->state_masks_[2 * rounds_].get()));

  for (unsigned int i = 0; i < rounds_; ++i) {
    this->sbox_layers_[i].reset(perm->sbox_layers_[i]->clone());
    this->sbox_layers_[i]->SetMasks(this->state_masks_[2*i].get(), this->state_masks_[2*i + 1].get());
    this->linear_layers_[i].reset(perm->linear_layers_[i]->clone());
    this->linear_layers_[i]->SetMasks(this->state_masks_[2*i + 1].get(), this->state_masks_[2*i + 2].get());
  }
  this->toupdate_linear = perm->toupdate_linear;
  this->toupdate_nonlinear = perm->toupdate_nonlinear;
}




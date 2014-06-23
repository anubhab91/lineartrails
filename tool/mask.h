#ifndef MASK_H_
#define MASK_H_

#include <vector>
#include <array>
#include <iostream>
#include <assert.h>
#include <functional>

#include "updatequeue.h"

#define BM_DUNNO  3
#define BM_0      2
#define BM_1      1
#define BM_CONTRA 0

typedef char BitMask;                  // mask for 1 bit (10=0, 01=1, 11=?)
typedef std::vector<BitMask> WordMask;
typedef uint64_t BitVector;            // n-bit vector




template <unsigned bitsize>
struct WordMaskPair {

  WordMaskPair(WordMask in, WordMask out) : in(in), out(out) { }
  bool operator==(const WordMaskPair<bitsize> &other) const {
    bool ret_val = true;
    for(int i = 0; i<bitsize; ++i){
      ret_val &= ((this->in[i] & 3) == (other.in[i] & 3));
      ret_val &= ((this->out[i] & 3) ==(other.out[i]& 3));
    }
    return ret_val;
  }

  WordMask in;
  WordMask out;
};

namespace std {
template<unsigned bitsize>
struct hash<WordMaskPair<bitsize>> {
  std::size_t operator()(const WordMaskPair<bitsize>& k) const {
    uint64_t v0, v1, v2, v3;
    uint64_t message;
    v0 = v1 = v2 = v3 = 0;

    for (int i = 0; i < bitsize; i += 16) {
      message = 0;
      for (int j = i; j < i + 16; ++j) {
        message <<= 2;
        //TODO: Maybe get rid of the & 2
        message |= (k.in[j] & 3);
        message <<= 2;
        message |= (k.out[j] & 3);
      }
      v0 ^= message;
      for (int j = 0; j < 2; j++) {
        v0 += v1;
        v2 += v3;
        v1 = (v1 << 13) | (v1 >> (64 - 13));
        v3 = (v3 << 16) | (v3 >> (64 - 16));
        v1 ^= v0;
        v3 ^= v2;
        v0 = (v0 << 32) | (v0 >> 32);
        v2 += v1;
        v0 += v3;
        v1 = (v1 << 17) | (v1 >> (64 - 17));
        v3 = (v3 << 21) | (v3 >> (64 - 21));
        v1 ^= v2;
        v3 ^= v0;
        v2 = (v2 << 32) | (v2 >> 32);
      }
      v3 ^= message;
    }

    return v0 ^ v1 ^ v2 ^ v3;
  }
};
}

struct WordMaskCare {
  WordMaskCare& operator=(const WordMaskCare& rhs);
  WordMaskCare(unsigned bitsize);
  WordMaskCare(const WordMaskCare& other);
  WordMaskCare(BitVector canbe1, BitVector care);
  void Reset();

  BitVector canbe1;                    // 1=canbe1, 0=mustbe1 // LSB = WordMask[0]!!!
  BitVector care;                      // 1=0/1, 0=?)
};

struct ProbabilityPair{
  char sign; //could have stored in one, but meh.
  double bias; //ld of bias
};

struct Mask {
  Mask& operator=(const Mask& rhs);
  Mask(unsigned bitsize);
  Mask(const Mask& other);
  Mask(std::initializer_list<char> other);
  Mask(WordMask& other);
  Mask(WordMaskCare& other);
  void init_caremask();
  void reinit_caremask();
  void init_bitmasks();
  void set_bit(BitMask bit, const int index);

  friend std::ostream& operator<<(std::ostream& stream, const Mask& mask);

  WordMask bitmasks; // maybe don't store this & use just for initialization
  WordMaskCare caremask;
};

struct StateMask {
  virtual StateMask* clone() = 0;
  virtual void print(std::ostream& stream) = 0;
  virtual void SetState(BitMask value) = 0;
  virtual void SetBit(BitMask value, int word_pos, int bit_pos) = 0;
  virtual std::vector<UpdatePos> diff(const StateMask& other) = 0;
  virtual Mask& operator[](const int index) = 0;
  virtual const Mask& operator[](const int index) const = 0;
};

#endif // MASK_H_

#ifndef ARM64_ARM64_CONVENTION_H_
#define ARM64_ARM64_CONVENTION_H_

#include <base/primitive_types.h>
#include "assembler/vixl/aarch64/operands-aarch64.h"

namespace whale {
namespace arm64 {

enum XRegister {
    X0 = 0,
    X1 = 1,
    X2 = 2,
    X3 = 3,
    X4 = 4,
    X5 = 5,
    X6 = 6,
    X7 = 7,
    X8 = 8,
    X9 = 9,
    X10 = 10,
    X11 = 11,
    X12 = 12,
    X13 = 13,
    X14 = 14,
    X15 = 15,
    X16 = 16,
    X17 = 17,
    X18 = 18,
    X19 = 19,
    X20 = 20,
    X21 = 21,
    X22 = 22,
    X23 = 23,
    X24 = 24,
    X25 = 25,
    X26 = 26,
    X27 = 27,
    X28 = 28,
    X29 = 29,
    X30 = 30,
    SP = 31,
    XZR = 32,
    kNumberOfXRegisters = 33,
    FP = X29,
    LR = X30,
    kNoRegister = -1,
};

enum WRegister {
    W0 = 0,
    W1 = 1,
    W2 = 2,
    W3 = 3,
    W4 = 4,
    W5 = 5,
    W6 = 6,
    W7 = 7,
    W8 = 8,
    W9 = 9,
    W10 = 10,
    W11 = 11,
    W12 = 12,
    W13 = 13,
    W14 = 14,
    W15 = 15,
    W16 = 16,
    W17 = 17,
    W18 = 18,
    W19 = 19,
    W20 = 20,
    W21 = 21,
    W22 = 22,
    W23 = 23,
    W24 = 24,
    W25 = 25,
    W26 = 26,
    W27 = 27,
    W28 = 28,
    W29 = 29,
    W30 = 30,
    WSP = 31,
    WZR = 32,
    kNumberOfWRegisters = 33,
    kNoWRegister = -1,
};

enum DRegister {
    D0 = 0,
    D1 = 1,
    D2 = 2,
    D3 = 3,
    D4 = 4,
    D5 = 5,
    D6 = 6,
    D7 = 7,
    D8 = 8,
    D9 = 9,
    D10 = 10,
    D11 = 11,
    D12 = 12,
    D13 = 13,
    D14 = 14,
    D15 = 15,
    D16 = 16,
    D17 = 17,
    D18 = 18,
    D19 = 19,
    D20 = 20,
    D21 = 21,
    D22 = 22,
    D23 = 23,
    D24 = 24,
    D25 = 25,
    D26 = 26,
    D27 = 27,
    D28 = 28,
    D29 = 29,
    D30 = 30,
    D31 = 31,
    kNumberOfDRegisters = 32,
    kNoDRegister = -1,
};

std::ostream &operator<<(std::ostream &os, const DRegister &rhs);

enum SRegister {
    S0 = 0,
    S1 = 1,
    S2 = 2,
    S3 = 3,
    S4 = 4,
    S5 = 5,
    S6 = 6,
    S7 = 7,
    S8 = 8,
    S9 = 9,
    S10 = 10,
    S11 = 11,
    S12 = 12,
    S13 = 13,
    S14 = 14,
    S15 = 15,
    S16 = 16,
    S17 = 17,
    S18 = 18,
    S19 = 19,
    S20 = 20,
    S21 = 21,
    S22 = 22,
    S23 = 23,
    S24 = 24,
    S25 = 25,
    S26 = 26,
    S27 = 27,
    S28 = 28,
    S29 = 29,
    S30 = 30,
    S31 = 31,
    kNumberOfSRegisters = 32,
    kNoSRegister = -1,
};

enum VRegister {
    V0 = 0,
    V1 = 1,
    V2 = 2,
    V3 = 3,
    V4 = 4,
    V5 = 5,
    V6 = 6,
    V7 = 7,
    V8 = 8,
    V9 = 9,
    V10 = 10,
    V11 = 11,
    V12 = 12,
    V13 = 13,
    V14 = 14,
    V15 = 15,
    V16 = 16,
    V17 = 17,
    V18 = 18,
    V19 = 19,
    V20 = 20,
    V21 = 21,
    V22 = 22,
    V23 = 23,
    V24 = 24,
    V25 = 25,
    V26 = 26,
    V27 = 27,
    V28 = 28,
    V29 = 29,
    V30 = 30,
    V31 = 31,
    kNumberOfVRegisters = 32,
    kNoVRegister = -1,
};

static vixl::aarch64::XRegister xTarget = vixl::aarch64::x17;


inline const vixl::aarch64::Register &XReg(unsigned int reg) {
    return vixl::aarch64::XRegister::GetXRegFromCode(reg);
}

inline const vixl::aarch64::Register &WReg(unsigned int reg) {
    return vixl::aarch64::XRegister::GetWRegFromCode(reg);
}

inline const vixl::aarch64::VRegister &SReg(unsigned int reg) {
    return vixl::aarch64::FPRegister::GetSRegFromCode(reg);
}

inline const vixl::aarch64::VRegister &DReg(unsigned int reg) {
    return vixl::aarch64::FPRegister::GetDRegFromCode(reg);
}

inline const vixl::aarch64::VRegister &VReg(unsigned int reg) {
    return vixl::aarch64::FPRegister::GetVRegFromCode(reg);
}

}  // namespace arm64
}  // namespace whale


#endif  // ARM64_ARM64_CONVENTION_H_



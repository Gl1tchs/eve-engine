#pragma once

#include "pch.h"

#ifdef EVE_PLATFORM_WIN32
#include <intrin.h>
#endif

class CPUInfo {
  class CPUID {
    uint32_t regs_[4] = {0};

   public:
    explicit CPUID(uint32_t funcId, uint32_t subFuncId) {
#ifdef EVE_PLATFORM_WIN32
      ::__cpuidex((int32_t*)regs_, (int32_t)funcId, (int32_t)subFuncId);
#else
      asm volatile("cpuid"
                   : "=a"(regs_[0]), "=b"(regs_[1]), "=c"(regs_[2]),
                     "=d"(regs_[3])
                   : "a"(funcId), "c"(subFuncId));
      // ECX is set to zero for CPUID function 4
#endif
    }

    const uint32_t& EBX() const { return regs_[1]; }
    const uint32_t& EAX() const { return regs_[0]; }
    const uint32_t& ECX() const { return regs_[2]; }
    const uint32_t& EDX() const { return regs_[3]; }
  };

 public:
  CPUInfo();

  std::string GetVendor() const { return vendor_id_; }
  std::string GetModel() const { return model_name_; }

  int32_t GetCores() const { return num_cores_; }
  // WARNING! CPUID reports hardware CAPABILITIES. For Intel CPUs you will still get HT=on and logicalCpus() > cores() even if HT is OFF in the BIOS.
  // Query the OS for actual correct runtime info.
  int32_t GetLogicalCpuCount() const { return num_log_cpus_; }
  bool IsHyperThreaded() const { return is_htt_; }
  bool IsSSE() const { return is_sse_; }
  bool IsSSE2() const { return is_sse2_; }
  bool IsSSE3() const { return is_sse3_; }
  bool IsSSE41() const { return is_sse43_; }
  bool IsSSE42() const { return is_sse42_; }
  bool IsAVX() const { return is_avx_; }
  bool IsAVX2() const { return is_avx2_; }
  bool IsAVX512F() const { return is_avx_512f_; }

 private:
  // Bit positions for data extractions
  static constexpr uint32_t SSE_POS = 0x02000000;
  static constexpr uint32_t SSE2_POS = 0x04000000;
  static constexpr uint32_t SSE3_POS = 0x00000001;
  static constexpr uint32_t SSE41_POS = 0x00080000;
  static constexpr uint32_t SSE42_POS = 0x00100000;
  static constexpr uint32_t AVX_POS = 0x10000000;
  static constexpr uint32_t AVX2_POS = 0x00000020;
  static constexpr uint32_t AVX512F_POS = 1u << 15;  // Bit 16
  static constexpr uint32_t LVL_NUM = 0x000000FF;
  static constexpr uint32_t LVL_TYPE = 0x0000FF00;
  static constexpr uint32_t LVL_CORES = 0x0000FFFF;

  // Attributes
  std::string vendor_id_;
  std::string model_name_;
  int32_t num_smt_ = 0;
  int32_t num_cores_ = 0;
  int32_t num_log_cpus_ = 0;
  bool is_htt_ = 0;
  bool is_sse_ = false;
  bool is_sse2_ = false;
  bool is_sse3_ = false;
  bool is_sse43_ = false;
  bool is_sse42_ = false;
  bool is_avx_ = false;
  bool is_avx2_ = false;
  bool is_avx_512f_ = false;
};

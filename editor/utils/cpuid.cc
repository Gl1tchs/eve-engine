#include "utils/cpuid.h"

CPUInfo::CPUInfo() {
  // Get vendor name EAX=0
  CPUID cpu_id0(0, 0);
  const uint32_t HFS = cpu_id0.EAX();
  // Reinterpret bytes as ASCII characters
  vendor_id_ += std::string((const char*)&cpu_id0.EBX(), 4);
  vendor_id_ += std::string((const char*)&cpu_id0.EDX(), 4);
  vendor_id_ += std::string((const char*)&cpu_id0.ECX(), 4);
  // Get SSE instructions availability
  CPUID cpu_id1(1, 0);
  is_htt_ = cpu_id1.EDX() & AVX_POS;
  is_sse_ = cpu_id1.EDX() & SSE_POS;
  is_sse2_ = cpu_id1.EDX() & SSE2_POS;
  is_sse3_ = cpu_id1.ECX() & SSE3_POS;
  is_sse43_ = cpu_id1.ECX() & SSE41_POS;
  is_sse42_ = cpu_id1.ECX() & SSE41_POS;
  is_avx_ = cpu_id1.ECX() & AVX_POS;
  // Get AVX2 instructions availability
  CPUID cpu_id7(7, 0);
  is_avx2_ = cpu_id7.EBX() & AVX2_POS;
  is_avx_512f_ = cpu_id7.EBX() & AVX512F_POS;

  std::string vendor_id_uppercase = vendor_id_;
  std::for_each(vendor_id_uppercase.begin(), vendor_id_uppercase.end(),
                [](char& character) {
                  character = static_cast<char>(::toupper(character));
                });
  // Get num of cores
  if (vendor_id_uppercase.find("INTEL") != std::string::npos) {
    if (HFS >= 11) {
      static constexpr int32_t MAX_INTEL_TOP_LVL = 4;
      for (int32_t lvl = 0; lvl < MAX_INTEL_TOP_LVL; ++lvl) {
        CPUID cpu_id4(0x0B, lvl);
        uint32_t currLevel = (LVL_TYPE & cpu_id4.ECX()) >> 8;
        switch (currLevel) {
          case 0x01:
            num_smt_ = LVL_CORES & cpu_id4.EBX();
            break;  //  EAX=0xB, ECX=0 - EBX is the number of logical processors (threads) per core
          case 0x02:
            num_log_cpus_ = LVL_CORES & cpu_id4.EBX();
            break;  // EAX=0xB, ECX=1 - EBX is the number of logical processors per processor package
          default:
            break;
        }
      }
      num_cores_ = num_log_cpus_ / num_smt_;
      is_htt_ = num_smt_ > 1;
    } else {
      if (HFS >= 1) {
        num_log_cpus_ = (cpu_id1.EBX() >> 16) & 0xFF;
        if (HFS >= 4) {
          num_cores_ = 1 + (CPUID(4, 0).EAX() >> 26) & 0x3F;
        }
      }
      if (is_htt_) {
        if (!(num_cores_ > 1)) {
          num_cores_ = 1;
          num_log_cpus_ = (num_log_cpus_ >= 2 ? num_log_cpus_ : 2);
        }
      } else {
        num_cores_ = num_log_cpus_ = 1;
      }
    }
  } else if (vendor_id_uppercase.find("AMD") != std::string::npos) {
    // From https://github.com/time-killer-games/ween/blob/db69cafca2222c634a1d3a9e58262b5a2dc8d508/system.cpp#L1469-L1528
    num_smt_ = 1 + ((CPUID(0x8000001e, 0).EBX() >> 8) & 0xff);
    if (num_log_cpus_ > 0 && num_smt_ > 0) {
      num_cores_ = num_log_cpus_ / num_smt_;
    } else {
      if (HFS >= 1) {
        if (CPUID(0x80000000, 0).EAX() >= 8) {
          num_cores_ = 1 + (CPUID(0x80000008, 0).ECX() & 0xFF);
        }
      }
      if (is_htt_) {
        if (num_cores_ < 1) {
          num_cores_ = 1;
        }
      } else {
        num_cores_ = 1;
      }
    }
  } else {
    throw std::runtime_error{"Unknown vendor! Reported vendor name is: " +
                             vendor_id_};
  }

  // Get processor brand string
  // This seems to be working for both Intel & AMD vendors
  for (int i = 0x80000002; i < 0x80000005; ++i) {
    CPUID cpu_id(i, 0);
    model_name_ += std::string((const char*)&cpu_id.EAX(), 4);
    model_name_ += std::string((const char*)&cpu_id.EBX(), 4);
    model_name_ += std::string((const char*)&cpu_id.ECX(), 4);
    model_name_ += std::string((const char*)&cpu_id.EDX(), 4);
  }
}
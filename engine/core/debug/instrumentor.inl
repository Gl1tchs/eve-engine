// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

namespace instrumentor_utils {

template <size_t N, size_t K>
inline constexpr auto CleanupOutputString(const char (&expr)[N],
                                          const char (&remove)[K]) {
  ChangeResult<N> result = {};

  size_t src_index = 0;
  size_t dst_index = 0;
  while (src_index < N) {
    size_t match_index = 0;
    while (match_index < K - 1 && src_index + match_index < N - 1 &&
           expr[src_index + match_index] == remove[match_index])
      match_index++;
    if (match_index == K - 1)
      src_index += match_index;
    result.data[dst_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
    src_index++;
  }
  return result;
}

}  // namespace instrumentor_utils
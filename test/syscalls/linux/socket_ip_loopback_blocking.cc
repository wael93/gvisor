// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <netinet/tcp.h>
#include <vector>

#include "test/syscalls/linux/ip_socket_test_util.h"
#include "test/syscalls/linux/socket_blocking.h"
#include "test/syscalls/linux/socket_test_util.h"
#include "test/util/test_util.h"

namespace gvisor {
namespace testing {

std::vector<SocketPairKind> GetSocketPairs() {
  return VecCat<SocketPairKind>(
      ApplyVec<SocketPairKind>(
          IPv6UDPBidirectionalBindSocketPair,
          AllBitwiseCombinations(List<int>{0, SOCK_CLOEXEC})),
      ApplyVec<SocketPairKind>(
          IPv4UDPBidirectionalBindSocketPair,
          AllBitwiseCombinations(List<int>{0, SOCK_CLOEXEC})),
      ApplyVecToVec<SocketPairKind>(
          std::vector<Middleware>{
              NoOp, SetSockOpt(IPPROTO_TCP, TCP_NODELAY, &kSockOptOn)},
          VecCat<SocketPairKind>(
              ApplyVec<SocketPairKind>(
                  IPv6TCPAcceptBindSocketPair,
                  AllBitwiseCombinations(List<int>{0, SOCK_CLOEXEC})),
              ApplyVec<SocketPairKind>(
                  IPv4TCPAcceptBindSocketPair,
                  AllBitwiseCombinations(List<int>{0, SOCK_CLOEXEC})))));
}

INSTANTIATE_TEST_CASE_P(
    AllUnixDomainSockets, BlockingSocketPairTest,
    ::testing::ValuesIn(IncludeReversals(GetSocketPairs())));

}  // namespace testing
}  // namespace gvisor
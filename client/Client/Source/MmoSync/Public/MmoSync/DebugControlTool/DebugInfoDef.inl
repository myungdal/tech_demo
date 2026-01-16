// ========================================================================
// DebugInfo 변수 정의 (X-Macro 패턴)
//
// - 이 파일 하나에서 선언, 정의, 등록이 모두 생성됩니다.
// - 값 변경 시 이 파일만 수정하면 됩니다 (cpp만 재컴파일)
// ========================================================================

// ========================================================================
// 디버그 설정
// ========================================================================
DEBUG_VAR_DEF(DEBUG_GAME_DIRECTLY, int, 0)                                      // 게임 직접 시작 (로비 스킵)
DEBUG_VAR_DEF(DEBUG_ENTITY_ID, EntityId, -1)                                    // 브레이크포인트 대상 엔티티 ID (-1이면 무시)
DEBUG_VAR_DEF(DEBUG_STEP_BNT_COUNT, int, 1)                                     // 디버그 위젯 스텝 버튼 클릭 시 진행할 프레임 수
DEBUG_VAR_DEF(DEBUG_DAMAGE, StatValue, -1)                                      // 고정 데미지 값 (-1이면 정상 계산)
DEBUG_VAR_DEF(DEBUG_TEST_ENTITY_CNT, int, 1)                                    // 테스트용 엔티티 생성 개수
DEBUG_VAR_DEF(DEBUG_TEST_ENTITY_HP, int, 10000)                                 // 테스트용 엔티티 HP
DEBUG_VAR_DEF(DEBUG_TEST_GROUND_VEKOCITY, int, 30)                              // 테스트용 지상 이동 속도
DEBUG_VAR_DEF(DEBUG_OFFLINE_PLAY, int, 1)                                       // 오프라인 플레이 모드 (서버 연결 없이 테스트)
DEBUG_VAR_DEF(DEBUG_PICK_ENTITY_MODE, int, 1)                                   // 엔티티 선택 모드 (0:점, 1:원, 2:OBB, 3:섹터, 4:체스판)
DEBUG_VAR_DEF(DEBUG_SMOOTH_ENABLED, int, 1)                                     // 위치/각도 스무딩 활성화 (0:비활성화, 1:활성화)

// ========================================================================
// 렌더링 옵션
// ========================================================================
DEBUG_VAR_DEF(RENDER_TILE, int, 1)                                              // 타일 렌더링 on/off
DEBUG_VAR_DEF(RENDER_GRID, int, 0)                                              // 그리드 렌더링 on/off
DEBUG_VAR_DEF(RENDER_CHARACTER_MODE, int, 1)                                    // 캐릭터 렌더링 모드 (0:2D단색회전, 1:2D텍스쳐회전, 2:2D텍스쳐반전, 3:3D)
DEBUG_VAR_DEF(RENDER_CHARACTER_SCALE, int, 1000)                                 // 캐릭터 텍스처 배율 (1000=1.0배, 500=0.5배, 1500=1.5배)
DEBUG_VAR_DEF(RENDER_CHARACTER_FLIP_HYSTERESIS, int, 5)                         // 좌우반전 히스테리시스 (도 단위, 떨림 방지)
DEBUG_VAR_DEF(RENDER_BLOCKED, int, 1)                                           // 막힌 영역 렌더링 on/off
DEBUG_VAR_DEF(RENDER_FLOW, int, 0)                                              // 플로우필드 렌더링 on/off
DEBUG_VAR_DEF(RENDER_ASTAR, int, 0)                                             // A* 경로 렌더링 on/off
DEBUG_VAR_DEF(RENDER_VISIBILITY, int, 0)                                        // 가시성 영역 렌더링 on/off
DEBUG_VAR_DEF(RENDER_INVISIBILITY, int, 1)                                      // 비가시 영역 렌더링 on/off
DEBUG_VAR_DEF(RENDER_VIRTUAL_CENTER, int, 1)                                    // 가상 중심점 표시 (0:끔, 1:켬)
DEBUG_VAR_DEF(RENDER_CHARACTER_COLOR, int, 1)                                   // 캐릭터 색상 마커 표시 (0:끔, 1:켬)
DEBUG_VAR_DEF(MINIMAP_SHOW_ALLY, int, 1)                                        // 미니맵 아군 표시 (PC)
DEBUG_VAR_DEF(MINIMAP_SHOW_ENEMY, int, 0)                                       // 미니맵 적군 표시 (NPC)

// ========================================================================
// 이펙트
// ========================================================================
DEBUG_VAR_DEF(EFFECT_DURATION, int, 200)                                        // 이펙트 지속 시간 (ms)

// ========================================================================
// 물리 시스템
// ========================================================================
DEBUG_VAR_DEF(PHYSICS_GROUND_STATIC_FRICTION, int64_t, 30000000)                // 지면 정지 마찰력
DEBUG_VAR_DEF(PHYSICS_GROUND_STOP_VELOCITY, int64_t, 3000000)                   // 지면 정지 판정 속도
DEBUG_VAR_DEF(PHYSICS_GROUND_FORCE, int64_t, 0)                                 // 지면 이동 힘
DEBUG_VAR_DEF(PHYSICS_GROUND_VELOCITY, int64_t, 15)                             // 지면 기본 속도
DEBUG_VAR_DEF(PHYSICS_GROUND_VELOCITY_SLOW, int64_t, 10)                        // 지면 감속 속도
DEBUG_VAR_DEF(PHYSICS_GROUND_FRICTION, int64_t, 5)                              // 지면 마찰 계수
DEBUG_VAR_DEF(PHYSICS_AIR_FORCE, int64_t, 1)                                    // 공중 이동 힘
DEBUG_VAR_DEF(PHYSICS_AIR_VELOCITY, int64_t, 0)                                 // 공중 기본 속도
DEBUG_VAR_DEF(PHYSICS_AIR_FRICTION, int64_t, 30)                                // 공중 마찰 계수
DEBUG_VAR_DEF(PHYSICS_AIR_FRICTION_SLOW, int64_t, 20)                           // 공중 감속 마찰
DEBUG_VAR_DEF(PHYSICS_DAMAGE_PUSH_BACK, int64_t, 600000)                       // 피격 시 밀려나는 힘
DEBUG_VAR_DEF(PHYSICS_DAMAGE_DYING, int64_t, 400000)                           // 사망 시 밀려나는 힘
DEBUG_VAR_DEF(PHYSICS_DAMAGE_NORMAL, int64_t, 0)                                // 일반 피격 밀림

// ========================================================================
// 이동 명령 (<< CELL_SIZE_DESIGN_FACTOR 적용)
// ========================================================================
DEBUG_VAR_DEF(MOVE_TO_LIMIT_FROM_CETNER_NEAR, int64_t, (3000 << CELL_SIZE_DESIGN_FACTOR))  // 중심점에서 가까운 이동 제한 거리
DEBUG_VAR_DEF(MOVE_TO_LIMIT_FROM_CETNER_FAR, int64_t, (5000 << CELL_SIZE_DESIGN_FACTOR))   // 중심점에서 먼 이동 제한 거리
DEBUG_VAR_DEF(MOVE_TO_ASTAR_LOD, int, 2)                                        // A* LOD 레벨
DEBUG_VAR_DEF(MOVE_TO_ASTAR_STEP, int, 128)                                     // A* 스텝 수
DEBUG_VAR_DEF(MOVE_TO_DISTANCE_BY_KEY, int64_t, (1000 << CELL_SIZE_DESIGN_FACTOR))         // 키 입력당 이동 거리
DEBUG_VAR_DEF(MOVE_TO_ARRIVAL_RANGE, Scalar, (100 << CELL_SIZE_DESIGN_FACTOR))             // 도착 판정 거리
DEBUG_VAR_DEF(MOVE_TO_REPEAT_COOLDOWN, int, 16)                                 // 이동 명령 반복 쿨다운 (프레임)

DEBUG_VAR_DEF(USER_MIN_HOLD, int64_t, (1000 >> TIME_STEP_INTERVAL_FACTOR))                 // 유저 명령 최소 유지 시간
DEBUG_VAR_DEF(USER_MAX_LIFETIME, int64_t, (3000 >> TIME_STEP_INTERVAL_FACTOR))             // 유저 명령 최대 수명

// ========================================================================
// 이동 레이어 (>> TIME_STEP_INTERVAL_FACTOR 적용)
// minHold: 도착 후 최소 유지 시간, maxLifetime: 명령 최대 수명
// ========================================================================
DEBUG_VAR_DEF(MOVE_LAYER_REACTIVE_MIN_HOLD, Span, (5 >> TIME_STEP_INTERVAL_FACTOR))        // REACTIVE 레이어 최소 유지 (넉백 등)
DEBUG_VAR_DEF(MOVE_LAYER_REACTIVE_MAX_LIFETIME, Span, (60 >> TIME_STEP_INTERVAL_FACTOR))   // REACTIVE 레이어 최대 수명
DEBUG_VAR_DEF(MOVE_LAYER_ACTION_MIN_HOLD, Span, (10 >> TIME_STEP_INTERVAL_FACTOR))         // ACTION 레이어 최소 유지 (애니메이션)
DEBUG_VAR_DEF(MOVE_LAYER_ACTION_MAX_LIFETIME, Span, (120 >> TIME_STEP_INTERVAL_FACTOR))    // ACTION 레이어 최대 수명
DEBUG_VAR_DEF(MOVE_LAYER_USER_MIN_HOLD, Span, 0)                                                // USER 레이어 최소 유지 (즉시 전환)
DEBUG_VAR_DEF(MOVE_LAYER_USER_MAX_LIFETIME, Span, (180 >> TIME_STEP_INTERVAL_FACTOR))      // USER 레이어 최대 수명
DEBUG_VAR_DEF(MOVE_LAYER_TACTICS_MIN_HOLD, Span, 0)                                             // TACTICS 레이어 최소 유지
DEBUG_VAR_DEF(MOVE_LAYER_TACTICS_MAX_LIFETIME, Span, (1 >> TIME_STEP_INTERVAL_FACTOR))     // TACTICS 레이어 최대 수명 (0=무제한)
DEBUG_VAR_DEF(MOVE_LAYER_FOLLOW_MIN_DISTANCE, Scalar, 0)                                        // 이전 명령과의 최소 거리 차이

// ========================================================================
// PC 리더/팔로워 및 진형 (Formation)
// ========================================================================
DEBUG_VAR_DEF(MAX_NPC_FOLLOWER, int, 20)                                         // 최대 포획 가능 NPC Follower 수
DEBUG_VAR_DEF(LEADER_TURN_SPEED, int64_t, 1024)                                 // 리더 회전 속도 (1024=100%)
DEBUG_VAR_DEF(LEADER_TURN_SPEED_MIN_FACTOR, int64_t, 1024)                       // 최소 감속 비율 (팔로워 많을 때)
DEBUG_VAR_DEF(LEADER_TURN_SPEED_SLOW_PER_FOLLOWER, int64_t, 0)                 // 팔로워 1명당 감속량
DEBUG_VAR_DEF(LEADER_DASH_DURATION, Span, (1000 >> TIME_STEP_INTERVAL_FACTOR))             // 리더 대시 지속 시간
DEBUG_VAR_DEF(LEADER_DASH_DISTANCE, int64_t, (2000 << CELL_SIZE_DESIGN_FACTOR))            // 리더 대시 거리
DEBUG_VAR_DEF(LEADER_DASH_IMPACT, int64_t, 600000)                                            // 리더 대시 임팩트
DEBUG_VAR_DEF(FOLLOWER_DASH_DURATION, Span, (1000 >> TIME_STEP_INTERVAL_FACTOR))           // 팔로워 대시 지속 시간
DEBUG_VAR_DEF(FOLLOWER_DASH_DISTANCE, int64_t, (2000 << CELL_SIZE_DESIGN_FACTOR))          // 팔로워 대시 거리
DEBUG_VAR_DEF(LEADER_ROLL_SPEED_RATE, int32_t, 2048)                                       // 리더 롤 속도 배율 (1024=100%, 2048=200%)
DEBUG_VAR_DEF(FOLLOWER_CHARGE_SPEED_RATE, int32_t, 2048)                                   // 팔로워 차지 속도 배율 (1024=100%, 2048=200%)
DEBUG_VAR_DEF(SKILL_DASH_SPEED_RATE, int32_t, 2048)                                        // 스킬 대시 속도 배율 (1024=100%, 1536=150%)
// Charge Target Blending (Charge 시 진형 중점을 리더 목표지점으로 이동 후 블렌딩)
DEBUG_VAR_DEF(CHARGE_BLEND_DELAY, Span, (2000 >> TIME_STEP_INTERVAL_FACTOR))              // 블렌딩 시작까지 유예 시간
DEBUG_VAR_DEF(CHARGE_BLEND_RATE, int32_t, 8)                                               // 프레임당 blendWeight 증가량 (1024=100%)
// ========================================================================
// 진형 - Crowd (밀집 대형)
// ========================================================================
DEBUG_VAR_DEF(CROWD_FORMATION_GAP, Scalar, (200 << CELL_SIZE_DESIGN_FACTOR))                 // 팔로워 간격 (직접 값 지정, 기본값: CELL_SIZE/2)
DEBUG_VAR_DEF(CROWD_FORMATION_FORCE_0, int64_t, 210)                            // 완전 겹침 시 밀어내는 힘
DEBUG_VAR_DEF(CROWD_FORMATION_FORCE_1, int64_t, 210)                            // 거리 유지용 밀고 당기는 힘
DEBUG_VAR_DEF(CROWD_FORMATION_DISTANCE_BEST_MIN, int64_t, (100 << CELL_SIZE_DESIGN_FACTOR))  // 최소 유지 거리 (가까우면 밀어냄)
DEBUG_VAR_DEF(CROWD_FORMATION_DISTANCE_BEST_MAX, int64_t, (1000 << CELL_SIZE_DESIGN_FACTOR)) // 최대 유지 거리 (멀면 당김)
DEBUG_VAR_DEF(CROWD_FORMATION_PATH_PREDICT_FRONT, int64_t, 1024)                // 앞쪽 파트너 회피 강도 (1024=100%)
DEBUG_VAR_DEF(CROWD_FORMATION_PATH_PREDICT_BACK, int64_t, 200)                  // 뒤쪽 파트너 회피 강도
DEBUG_VAR_DEF(CROWD_FORMATION_OVERLAP_FORCE_MULT, int64_t, 200)                 // 완전 겹침 시 힘 배율
DEBUG_VAR_DEF(CROWD_FORMATION_OVERLAP_MULT_MULT, int64_t, 200)                  // 겹침 정도 비례 추가 배율
DEBUG_VAR_DEF(CROWD_FORMATION_AVOID_WEIGHT_PUSH, int64_t, 8)                    // 충돌 방향 회피 가중치
DEBUG_VAR_DEF(CROWD_FORMATION_AVOID_WEIGHT_MOVE, int64_t, 1)                    // 목표 방향 전진 가중치 (교착 방지)
DEBUG_VAR_DEF(CROWD_FORMATION_AVOID_WEIGHT_RIGHT, int64_t, 1)                   // 오른쪽 비켜가기 가중치
DEBUG_VAR_DEF(CROWD_FORMATION_OPPOSITE_DIR_MULT, int64_t, 100)                  // 반대 방향 이동 시 밀어내는 힘 비율
DEBUG_VAR_DEF(CROWD_FORMATION_FORCE_2, int64_t, 10)                            // 최대 거리 초과 시 당기는 힘
// 가상 중심점 (Virtual Center) - 대장을 따르는 시스템 (Crowd용)
DEBUG_VAR_DEF(CROWD_VIRTUAL_CENTER_LEASH, Scalar, (3000 << CELL_SIZE_DESIGN_FACTOR))  // 끈 길이 (Leader와의 최대 거리)
DEBUG_VAR_DEF(CROWD_VIRTUAL_CENTER_PUSH, Scalar, (1500 << CELL_SIZE_DESIGN_FACTOR))   // 밀어내기 반지름 (Leader가 가까이 오면 비켜줌)
DEBUG_VAR_DEF(CROWD_VIRTUAL_CENTER_CONVERGE, Scalar, (0 << CELL_SIZE_DESIGN_FACTOR)) // 멈춤 시 수렴 속도 (프레임당 이동량)


// ========================================================================
// 진형 - Flock (무리 이동)
// ========================================================================
DEBUG_VAR_DEF(FLOCK_FORMATION_GAP, Scalar, (100 << CELL_SIZE_DESIGN_FACTOR))                 // 팔로워 간격
DEBUG_VAR_DEF(FLOCK_FORMATION_FORCE_0, int64_t, 210)                            // 겹침 회피 힘
DEBUG_VAR_DEF(FLOCK_FORMATION_FORCE_1, int64_t, 8000)                             // 정렬 힘
DEBUG_VAR_DEF(FLOCK_FORMATION_FORCE_2, int64_t, 1000)                             // 응집 힘
DEBUG_VAR_DEF(FLOCK_FORMATION_DISTANCE_BEST, int64_t, (1000 << CELL_SIZE_DESIGN_FACTOR))     // 최적 거리
// Flock 공전 설정
DEBUG_VAR_DEF(FLOCK_ORBIT_RADIUS_BASE, Scalar, (1000 << CELL_SIZE_DESIGN_FACTOR))             // 기본 궤도 반경
DEBUG_VAR_DEF(FLOCK_ORBIT_SPEED, int32_t, 100)                                               // 공전 속도 (프레임당 각도 변화, 65536=1회전)
// 가상 중심점 (Virtual Center) - 대장을 따르는 시스템 (Flock용)
DEBUG_VAR_DEF(FLOCK_VIRTUAL_CENTER_LEASH, Scalar, (2000 << CELL_SIZE_DESIGN_FACTOR))  // 끈 길이 (Leader와의 최대 거리)
DEBUG_VAR_DEF(FLOCK_VIRTUAL_CENTER_PUSH, Scalar, (0 << CELL_SIZE_DESIGN_FACTOR))   // 밀어내기 반지름 (Leader가 가까이 오면 비켜줌)
DEBUG_VAR_DEF(FLOCK_VIRTUAL_CENTER_CONVERGE, Scalar, (0 << CELL_SIZE_DESIGN_FACTOR)) // 멈춤 시 수렴 속도 (프레임당 이동량)

// 진형 규모 임계값 및 Small/Large 설정 제거됨 - 중규모(Medium)만 사용

// ========================================================================
// 전술 - 범위 (<< CELL_SIZE_DESIGN_FACTOR 적용)
// ========================================================================
DEBUG_VAR_DEF(TACTICS_COMBAT_RANGE, int64_t, (7000 << CELL_SIZE_DESIGN_FACTOR))            // 전투 탐지 범위
DEBUG_VAR_DEF(TACTICS_ROAMING_RANGE, int64_t, (3000 << CELL_SIZE_DESIGN_FACTOR))           // 배회 범위

// ========================================================================
// 전술 - 지속시간 (>> TIME_STEP_INTERVAL_FACTOR 적용)
// ========================================================================
DEBUG_VAR_DEF(TACTICS_ROAMING_DURATION_MIN, Span, (1000 >> TIME_STEP_INTERVAL_FACTOR))     // 배회 최소 시간
DEBUG_VAR_DEF(TACTICS_ROAMING_DURATION_MAX, Span, (3000 >> TIME_STEP_INTERVAL_FACTOR))     // 배회 최대 시간
DEBUG_VAR_DEF(TACTICS_RETURN_DURATION_LIMIT, Span, (7000 >> TIME_STEP_INTERVAL_FACTOR))    // 귀환 제한 시간
DEBUG_VAR_DEF(TACTICS_IDLE_DURATION_MIN, Span, (1000 >> TIME_STEP_INTERVAL_FACTOR))        // 대기 최소 시간
DEBUG_VAR_DEF(TACTICS_IDLE_DURATION_MAX, Span, (3000 >> TIME_STEP_INTERVAL_FACTOR))        // 대기 최대 시간
DEBUG_VAR_DEF(TACTICS_IDLE_DURATION_COMBAT, Span, (3000 >> TIME_STEP_INTERVAL_FACTOR))     // 전투 후 대기 시간
DEBUG_VAR_DEF(TACTICS_IDLE_DURATION_FLEE, Span, (3000 >> TIME_STEP_INTERVAL_FACTOR))       // 도주 후 대기 시간
DEBUG_VAR_DEF(TACTICS_MIN_HOLD, Span, (1000 >> TIME_STEP_INTERVAL_FACTOR))                 // 전술 최소 유지 시간
DEBUG_VAR_DEF(TACTICS_MAX_LIFETIME, Span, (3000 >> TIME_STEP_INTERVAL_FACTOR))             // 전술 최대 수명

// ========================================================================
// 탐지
// ========================================================================
DEBUG_VAR_DEF(DETECTION_DISTANCE, int64_t, (5000 << CELL_SIZE_DESIGN_FACTOR))              // 적 탐지 거리

// ========================================================================
// 회복/부활
// ========================================================================
DEBUG_VAR_DEF(RECOVERY_DEFAULT_HEAL, int64_t, 3)                                // 기본 회복량
DEBUG_VAR_DEF(REVIVE_PC, int64_t, 0)                                            // PC 부활 여부 (0:비활성, 1:활성)
DEBUG_VAR_DEF(REVIVE_NPC, int64_t, 1)                                           // NPC 부활 여부
DEBUG_VAR_DEF(REVIVE_SPAWNER, int64_t, 1)                                       // 스포너 부활 여부

// ========================================================================
// TimeStep 재생
// ========================================================================
DEBUG_VAR_DEF(TIMESTEP_CATCH_UP_MODE, int, 0)                                   // 따라잡기 모드 (0:부드러운 재생, 1:공격적 따라잡기)

// ========================================================================
// 모션 지속시간 (>> TIME_STEP_INTERVAL_FACTOR 적용)
// ========================================================================
DEBUG_VAR_DEF(MOTION_DURATION_DYING, Span, (1600 >> TIME_STEP_INTERVAL_FACTOR))            // 사망 애니메이션 시간
DEBUG_VAR_DEF(MOTION_DURATION_ARRIVAL, Span, (1600 >> TIME_STEP_INTERVAL_FACTOR))          // 도착 애니메이션 시간
DEBUG_VAR_DEF(MOTION_DURATION_DEAD_MIN, Span, (7000 >> TIME_STEP_INTERVAL_FACTOR))         // 사망 상태 최소 유지 (부활 전)
DEBUG_VAR_DEF(MOTION_DURATION_DEAD_MAX, Span, (8000 >> TIME_STEP_INTERVAL_FACTOR))         // 사망 상태 최대 유지
DEBUG_VAR_DEF(MOTION_DURATION_DAMAGE, Span, (200 >> TIME_STEP_INTERVAL_FACTOR))            // 피격 경직 시간
DEBUG_VAR_DEF(MOTION_DURATION_WEB, Span, (3000 >> TIME_STEP_INTERVAL_FACTOR))              // 거미줄 속박 시간

// ========================================================================
// 스포너 (>> TIME_STEP_INTERVAL_FACTOR 적용)
// ========================================================================
DEBUG_VAR_DEF(SPAWNER_MAX_COUNT, EntityCount, 10)                                // 최대 스폰 개수
DEBUG_VAR_DEF(SPAWNER_MAX_COST_TIME, int, (16000 >> TIME_STEP_INTERVAL_FACTOR))            // 최대 스폰 코스트 시간
DEBUG_VAR_DEF(SPAWNER_COST_TIME, int, (160 >> TIME_STEP_INTERVAL_FACTOR))                  // 스폰 1회당 코스트 시간
DEBUG_VAR_DEF(SPAWNER_COUNT_PENALTY, int, (1000 >> TIME_STEP_INTERVAL_FACTOR))             // 스폰 개수당 페널티 시간

// ========================================================================
// 기본 맵 데이터
// ========================================================================
DEBUG_VAR_DEF(DEFAULT_MAP_MAP_KIND, int, 1)                                     // 맵 종류 (0:던전, 1:필드)
DEBUG_VAR_DEF(DEFAULT_MAP_GRID_FACTOR, int, 4)                                  // 그리드 팩터 GRID_FLOW_CHUNK_MAP_SIZE_FACTOR = 4 이상으로 셋팅할 것
DEBUG_VAR_DEF(DEFAULT_MAP_CASTLE_CNT, int, 1)                                  // 성 개수
DEBUG_VAR_DEF(DEFAULT_MAP_NEST_CNT, int, 0)                                   // 둥지 개수
DEBUG_VAR_DEF(DEFAULT_MAP_NEST_FOLLOWER_MIN, int, 5)                            // 둥지당 최소 팔로워 수
DEBUG_VAR_DEF(DEFAULT_MAP_NEST_FOLLOWER_MAX, int, 8)                            // 둥지당 최대 팔로워 수
DEBUG_VAR_DEF(DEFAULT_MAP_NO_NEST_ZONE_FROM_EDGE, int, 4)                       // 가장자리 둥지 금지 구역 (셀)
DEBUG_VAR_DEF(DEFAULT_MAP_NO_NEST_ZONE_FROM_CENTER, int, 8)                     // 중앙 둥지 금지 구역 (셀)
DEBUG_VAR_DEF(DEFAULT_MAP_NO_NEST_ZONE_FROM_CASTLE, int, 16)                    // 성 주변 둥지 금지 구역 (셀)

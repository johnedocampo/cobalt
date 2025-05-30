// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_OPCODES_H_
#define V8_COMPILER_OPCODES_H_

#include <iosfwd>

#include "src/common/globals.h"

// Opcodes for control operators.
#define CONTROL_OP_LIST(V) \
  V(Start)                 \
  V(Loop)                  \
  V(Branch)                \
  V(Switch)                \
  V(IfTrue)                \
  V(IfFalse)               \
  V(IfSuccess)             \
  V(IfException)           \
  V(IfValue)               \
  V(IfDefault)             \
  V(Merge)                 \
  V(Deoptimize)            \
  V(DeoptimizeIf)          \
  V(DeoptimizeUnless)      \
  V(TrapIf)                \
  V(TrapUnless)            \
  V(Return)                \
  V(TailCall)              \
  V(Terminate)             \
  V(Throw)                 \
  V(End)

#define MACHINE_LEVEL_CONSTANT_OP_LIST(V) \
  V(Int32Constant)                        \
  V(Int64Constant)                        \
  V(TaggedIndexConstant)                  \
  V(Float32Constant)                      \
  V(Float64Constant)                      \
  V(CompressedHeapConstant)               \
  V(RelocatableInt32Constant)             \
  V(RelocatableInt64Constant)

#define JS_LEVEL_CONSTANT_OP_LIST(V) \
  V(ExternalConstant)                \
  V(NumberConstant)                  \
  V(PointerConstant)                 \
  V(HeapConstant)

// Opcodes for constant operators.
#define CONSTANT_OP_LIST(V)    \
  JS_LEVEL_CONSTANT_OP_LIST(V) \
  MACHINE_LEVEL_CONSTANT_OP_LIST(V)

#define INNER_OP_LIST(V)    \
  V(Select)                 \
  V(Phi)                    \
  V(EffectPhi)              \
  V(InductionVariablePhi)   \
  V(Checkpoint)             \
  V(BeginRegion)            \
  V(FinishRegion)           \
  V(FrameState)             \
  V(StateValues)            \
  V(TypedStateValues)       \
  V(ArgumentsElementsState) \
  V(ArgumentsLengthState)   \
  V(ObjectState)            \
  V(ObjectId)               \
  V(TypedObjectState)       \
  V(Call)                   \
  V(Parameter)              \
  V(OsrValue)               \
  V(LoopExit)               \
  V(LoopExitValue)          \
  V(LoopExitEffect)         \
  V(Projection)             \
  V(Retain)                 \
  V(MapGuard)               \
  V(TypeGuard)              \
  V(EnterMachineGraph)      \
  V(ExitMachineGraph)

#define COMMON_OP_LIST(V) \
  CONSTANT_OP_LIST(V)     \
  INNER_OP_LIST(V)        \
  V(Unreachable)          \
  V(DeadValue)            \
  V(Dead)                 \
  V(Plug)                 \
  V(SLVerifierHint)       \
  V(StaticAssert)

// Opcodes for JavaScript operators.
// Arguments are JSName (the name with a 'JS' prefix), and Name.
#define JS_COMPARE_BINOP_LIST(V)        \
  V(JSEqual, Equal)                     \
  V(JSStrictEqual, StrictEqual)         \
  V(JSLessThan, LessThan)               \
  V(JSGreaterThan, GreaterThan)         \
  V(JSLessThanOrEqual, LessThanOrEqual) \
  V(JSGreaterThanOrEqual, GreaterThanOrEqual)

#define JS_BITWISE_BINOP_LIST(V) \
  V(JSBitwiseOr, BitwiseOr)      \
  V(JSBitwiseXor, BitwiseXor)    \
  V(JSBitwiseAnd, BitwiseAnd)    \
  V(JSShiftLeft, ShiftLeft)      \
  V(JSShiftRight, ShiftRight)    \
  V(JSShiftRightLogical, ShiftRightLogical)

#define JS_ARITH_BINOP_LIST(V) \
  V(JSAdd, Add)                \
  V(JSSubtract, Subtract)      \
  V(JSMultiply, Multiply)      \
  V(JSDivide, Divide)          \
  V(JSModulus, Modulus)        \
  V(JSExponentiate, Exponentiate)

#define JS_SIMPLE_BINOP_LIST(V) \
  JS_COMPARE_BINOP_LIST(V)      \
  JS_BITWISE_BINOP_LIST(V)      \
  JS_ARITH_BINOP_LIST(V)        \
  V(JSHasInPrototypeChain)      \
  V(JSInstanceOf)               \
  V(JSOrdinaryHasInstance)

#define JS_CONVERSION_UNOP_LIST(V) \
  V(JSToLength)                    \
  V(JSToName)                      \
  V(JSToNumber)                    \
  V(JSToNumberConvertBigInt)       \
  V(JSToBigInt)                    \
  V(JSToBigIntConvertNumber)       \
  V(JSToNumeric)                   \
  V(JSToObject)                    \
  V(JSToString)                    \
  V(JSParseInt)

#define JS_BITWISE_UNOP_LIST(V) \
  V(JSBitwiseNot, BitwiseNot)   \
  V(JSNegate, Negate)

#define JS_ARITH_UNOP_LIST(V) \
  V(JSDecrement, Decrement)   \
  V(JSIncrement, Increment)

#define JS_SIMPLE_UNOP_LIST(V) \
  JS_ARITH_UNOP_LIST(V)        \
  JS_BITWISE_UNOP_LIST(V)      \
  JS_CONVERSION_UNOP_LIST(V)

#define JS_CREATE_OP_LIST(V)     \
  V(JSCloneObject)               \
  V(JSCreate)                    \
  V(JSCreateArguments)           \
  V(JSCreateArray)               \
  V(JSCreateArrayFromIterable)   \
  V(JSCreateArrayIterator)       \
  V(JSCreateAsyncFunctionObject) \
  V(JSCreateBoundFunction)       \
  V(JSCreateClosure)             \
  V(JSCreateCollectionIterator)  \
  V(JSCreateEmptyLiteralArray)   \
  V(JSCreateEmptyLiteralObject)  \
  V(JSCreateGeneratorObject)     \
  V(JSCreateIterResultObject)    \
  V(JSCreateKeyValueArray)       \
  V(JSCreateLiteralArray)        \
  V(JSCreateLiteralObject)       \
  V(JSCreateLiteralRegExp)       \
  V(JSCreateObject)              \
  V(JSCreatePromise)             \
  V(JSCreateStringIterator)      \
  V(JSCreateTypedArray)          \
  V(JSGetTemplateObject)

#define JS_OBJECT_OP_LIST(V)           \
  JS_CREATE_OP_LIST(V)                 \
  V(JSLoadProperty)                    \
  V(JSLoadNamed)                       \
  V(JSLoadNamedFromSuper)              \
  V(JSLoadGlobal)                      \
  V(JSSetKeyedProperty)                \
  V(JSDefineKeyedOwnProperty)          \
  V(JSSetNamedProperty)                \
  V(JSDefineNamedOwnProperty)          \
  V(JSStoreGlobal)                     \
  V(JSDefineKeyedOwnPropertyInLiteral) \
  V(JSStoreInArrayLiteral)             \
  V(JSDeleteProperty)                  \
  V(JSHasProperty)                     \
  V(JSGetSuperConstructor)             \
  V(JSFindNonDefaultConstructorOrConstruct)

#define JS_CONTEXT_OP_LIST(V) \
  V(JSHasContextExtension)    \
  V(JSLoadContext)            \
  V(JSStoreContext)           \
  V(JSCreateFunctionContext)  \
  V(JSCreateCatchContext)     \
  V(JSCreateWithContext)      \
  V(JSCreateBlockContext)

#define JS_CALL_OP_LIST(V) \
  V(JSCall)                \
  V(JSCallForwardVarargs)  \
  V(JSCallWithArrayLike)   \
  V(JSCallWithSpread)      \
  IF_WASM(V, JSWasmCall)

#define JS_CONSTRUCT_OP_LIST(V) \
  V(JSConstructForwardVarargs)  \
  V(JSConstruct)                \
  V(JSConstructWithArrayLike)   \
  V(JSConstructWithSpread)

#define JS_OTHER_OP_LIST(V)            \
  JS_CALL_OP_LIST(V)                   \
  JS_CONSTRUCT_OP_LIST(V)              \
  V(JSAsyncFunctionEnter)              \
  V(JSAsyncFunctionReject)             \
  V(JSAsyncFunctionResolve)            \
  V(JSCallRuntime)                     \
  V(JSForInEnumerate)                  \
  V(JSForInNext)                       \
  V(JSForInPrepare)                    \
  V(JSGetIterator)                     \
  V(JSLoadMessage)                     \
  V(JSStoreMessage)                    \
  V(JSLoadModule)                      \
  V(JSStoreModule)                     \
  V(JSGetImportMeta)                   \
  V(JSGeneratorStore)                  \
  V(JSGeneratorRestoreContinuation)    \
  V(JSGeneratorRestoreContext)         \
  V(JSGeneratorRestoreRegister)        \
  V(JSGeneratorRestoreInputOrDebugPos) \
  V(JSFulfillPromise)                  \
  V(JSPerformPromiseThen)              \
  V(JSPromiseResolve)                  \
  V(JSRejectPromise)                   \
  V(JSResolvePromise)                  \
  V(JSStackCheck)                      \
  V(JSObjectIsArray)                   \
  V(JSRegExpTest)                      \
  V(JSDebugger)

#define JS_OP_LIST(V)     \
  JS_SIMPLE_BINOP_LIST(V) \
  JS_SIMPLE_UNOP_LIST(V)  \
  JS_OBJECT_OP_LIST(V)    \
  JS_CONTEXT_OP_LIST(V)   \
  JS_OTHER_OP_LIST(V)

// Opcodes for VirtuaMachine-level operators.
#define SIMPLIFIED_CHANGE_OP_LIST(V) \
  V(ChangeTaggedSignedToInt32)       \
  V(ChangeTaggedSignedToInt64)       \
  V(ChangeTaggedToInt32)             \
  V(ChangeTaggedToInt64)             \
  V(ChangeTaggedToUint32)            \
  V(ChangeTaggedToFloat64)           \
  V(ChangeTaggedToTaggedSigned)      \
  V(ChangeInt31ToTaggedSigned)       \
  V(ChangeInt32ToTagged)             \
  V(ChangeInt64ToTagged)             \
  V(ChangeUint32ToTagged)            \
  V(ChangeUint64ToTagged)            \
  V(ChangeFloat64ToTagged)           \
  V(ChangeFloat64ToTaggedPointer)    \
  V(ChangeTaggedToBit)               \
  V(ChangeBitToTagged)               \
  V(ChangeInt64ToBigInt)             \
  V(ChangeUint64ToBigInt)            \
  V(TruncateBigIntToWord64)          \
  V(TruncateTaggedToWord32)          \
  V(TruncateTaggedToFloat64)         \
  V(TruncateTaggedToBit)             \
  V(TruncateTaggedPointerToBit)

#define SIMPLIFIED_CHECKED_OP_LIST(V) \
  V(CheckedInt32Add)                  \
  V(CheckedInt32Sub)                  \
  V(CheckedInt32Div)                  \
  V(CheckedInt32Mod)                  \
  V(CheckedUint32Div)                 \
  V(CheckedUint32Mod)                 \
  V(CheckedInt32Mul)                  \
  V(CheckedInt64Add)                  \
  V(CheckedInt64Sub)                  \
  V(CheckedInt64Mul)                  \
  V(CheckedInt64Div)                  \
  V(CheckedInt64Mod)                  \
  V(CheckedInt32ToTaggedSigned)       \
  V(CheckedInt64ToInt32)              \
  V(CheckedInt64ToTaggedSigned)       \
  V(CheckedUint32Bounds)              \
  V(CheckedUint32ToInt32)             \
  V(CheckedUint32ToTaggedSigned)      \
  V(CheckedUint64Bounds)              \
  V(CheckedUint64ToInt32)             \
  V(CheckedUint64ToInt64)             \
  V(CheckedUint64ToTaggedSigned)      \
  V(CheckedFloat64ToInt32)            \
  V(CheckedFloat64ToInt64)            \
  V(CheckedTaggedSignedToInt32)       \
  V(CheckedTaggedToInt32)             \
  V(CheckedTaggedToArrayIndex)        \
  V(CheckedTruncateTaggedToWord32)    \
  V(CheckedTaggedToFloat64)           \
  V(CheckedTaggedToInt64)             \
  V(CheckedTaggedToTaggedSigned)      \
  V(CheckedTaggedToTaggedPointer)

#define SIMPLIFIED_COMPARE_BINOP_LIST(V) \
  V(NumberEqual)                         \
  V(NumberLessThan)                      \
  V(NumberLessThanOrEqual)               \
  V(SpeculativeNumberEqual)              \
  V(SpeculativeNumberLessThan)           \
  V(SpeculativeNumberLessThanOrEqual)    \
  V(ReferenceEqual)                      \
  V(SameValue)                           \
  V(SameValueNumbersOnly)                \
  V(NumberSameValue)                     \
  V(StringEqual)                         \
  V(StringLessThan)                      \
  V(StringLessThanOrEqual)               \
  V(BigIntEqual)                         \
  V(BigIntLessThan)                      \
  V(BigIntLessThanOrEqual)               \
  V(SpeculativeBigIntEqual)              \
  V(SpeculativeBigIntLessThan)           \
  V(SpeculativeBigIntLessThanOrEqual)

#define SIMPLIFIED_NUMBER_BINOP_LIST(V) \
  V(NumberAdd)                          \
  V(NumberSubtract)                     \
  V(NumberMultiply)                     \
  V(NumberDivide)                       \
  V(NumberModulus)                      \
  V(NumberBitwiseOr)                    \
  V(NumberBitwiseXor)                   \
  V(NumberBitwiseAnd)                   \
  V(NumberShiftLeft)                    \
  V(NumberShiftRight)                   \
  V(NumberShiftRightLogical)            \
  V(NumberAtan2)                        \
  V(NumberImul)                         \
  V(NumberMax)                          \
  V(NumberMin)                          \
  V(NumberPow)

#define SIMPLIFIED_BIGINT_BINOP_LIST(V) \
  V(BigIntAdd)                          \
  V(BigIntSubtract)                     \
  V(BigIntMultiply)                     \
  V(BigIntDivide)                       \
  V(BigIntModulus)                      \
  V(BigIntBitwiseAnd)                   \
  V(BigIntBitwiseOr)                    \
  V(BigIntBitwiseXor)                   \
  V(BigIntShiftLeft)                    \
  V(BigIntShiftRight)

#define SIMPLIFIED_SPECULATIVE_NUMBER_BINOP_LIST(V) \
  V(SpeculativeNumberAdd)                           \
  V(SpeculativeNumberSubtract)                      \
  V(SpeculativeNumberMultiply)                      \
  V(SpeculativeNumberPow)                           \
  V(SpeculativeNumberDivide)                        \
  V(SpeculativeNumberModulus)                       \
  V(SpeculativeNumberBitwiseAnd)                    \
  V(SpeculativeNumberBitwiseOr)                     \
  V(SpeculativeNumberBitwiseXor)                    \
  V(SpeculativeNumberShiftLeft)                     \
  V(SpeculativeNumberShiftRight)                    \
  V(SpeculativeNumberShiftRightLogical)             \
  V(SpeculativeSafeIntegerAdd)                      \
  V(SpeculativeSafeIntegerSubtract)

#define SIMPLIFIED_NUMBER_UNOP_LIST(V) \
  V(NumberAbs)                         \
  V(NumberAcos)                        \
  V(NumberAcosh)                       \
  V(NumberAsin)                        \
  V(NumberAsinh)                       \
  V(NumberAtan)                        \
  V(NumberAtanh)                       \
  V(NumberCbrt)                        \
  V(NumberCeil)                        \
  V(NumberClz32)                       \
  V(NumberCos)                         \
  V(NumberCosh)                        \
  V(NumberExp)                         \
  V(NumberExpm1)                       \
  V(NumberFloor)                       \
  V(NumberFround)                      \
  V(NumberLog)                         \
  V(NumberLog1p)                       \
  V(NumberLog2)                        \
  V(NumberLog10)                       \
  V(NumberRound)                       \
  V(NumberSign)                        \
  V(NumberSin)                         \
  V(NumberSinh)                        \
  V(NumberSqrt)                        \
  V(NumberTan)                         \
  V(NumberTanh)                        \
  V(NumberTrunc)                       \
  V(NumberToBoolean)                   \
  V(NumberToInt32)                     \
  V(NumberToString)                    \
  V(NumberToUint32)                    \
  V(NumberToUint8Clamped)              \
  V(Integral32OrMinusZeroToBigInt)     \
  V(NumberSilenceNaN)

#define SIMPLIFIED_BIGINT_UNOP_LIST(V) \
  V(BigIntNegate)                      \
  V(CheckBigInt)                       \
  V(CheckedBigIntToBigInt64)

#define SIMPLIFIED_SPECULATIVE_NUMBER_UNOP_LIST(V) V(SpeculativeToNumber)

#define SIMPLIFIED_OTHER_OP_LIST(V)     \
  V(Allocate)                           \
  V(AllocateRaw)                        \
  V(ArgumentsLength)                    \
  V(AssertType)                         \
  V(BooleanNot)                         \
  V(CheckBounds)                        \
  V(CheckClosure)                       \
  V(CheckEqualsInternalizedString)      \
  V(CheckEqualsSymbol)                  \
  V(CheckFloat64Hole)                   \
  V(CheckHeapObject)                    \
  V(CheckIf)                            \
  V(CheckInternalizedString)            \
  V(CheckMaps)                          \
  V(CheckNotTaggedHole)                 \
  V(CheckNumber)                        \
  V(CheckReceiver)                      \
  V(CheckReceiverOrNullOrUndefined)     \
  V(CheckSmi)                           \
  V(CheckString)                        \
  V(CheckSymbol)                        \
  V(CheckTurboshaftTypeOf)              \
  V(CompareMaps)                        \
  V(ConvertReceiver)                    \
  V(ConvertTaggedHoleToUndefined)       \
  V(DateNow)                            \
  V(DoubleArrayMax)                     \
  V(DoubleArrayMin)                     \
  V(EnsureWritableFastElements)         \
  V(FastApiCall)                        \
  V(FindOrderedHashMapEntry)            \
  V(FindOrderedHashMapEntryForInt32Key) \
  V(FindOrderedHashSetEntry)            \
  V(InitializeImmutableInObject)        \
  V(LoadDataViewElement)                \
  V(LoadElement)                        \
  V(LoadField)                          \
  V(LoadFieldByIndex)                   \
  V(LoadFromObject)                     \
  V(LoadImmutableFromObject)            \
  V(LoadMessage)                        \
  V(LoadStackArgument)                  \
  V(LoadTypedElement)                   \
  V(MaybeGrowFastElements)              \
  V(NewArgumentsElements)               \
  V(NewConsString)                      \
  V(NewDoubleElements)                  \
  V(NewSmiOrObjectElements)             \
  V(NumberIsFinite)                     \
  V(NumberIsFloat64Hole)                \
  V(NumberIsInteger)                    \
  V(NumberIsMinusZero)                  \
  V(NumberIsNaN)                        \
  V(NumberIsSafeInteger)                \
  V(ObjectIsArrayBufferView)            \
  V(ObjectIsBigInt)                     \
  V(ObjectIsCallable)                   \
  V(ObjectIsConstructor)                \
  V(ObjectIsDetectableCallable)         \
  V(ObjectIsFiniteNumber)               \
  V(ObjectIsInteger)                    \
  V(ObjectIsMinusZero)                  \
  V(ObjectIsNaN)                        \
  V(ObjectIsNonCallable)                \
  V(ObjectIsNumber)                     \
  V(ObjectIsReceiver)                   \
  V(ObjectIsSafeInteger)                \
  V(ObjectIsSmi)                        \
  V(ObjectIsString)                     \
  V(ObjectIsSymbol)                     \
  V(ObjectIsUndetectable)               \
  V(PlainPrimitiveToFloat64)            \
  V(PlainPrimitiveToNumber)             \
  V(PlainPrimitiveToWord32)             \
  V(RestLength)                         \
  V(RuntimeAbort)                       \
  V(StoreDataViewElement)               \
  V(StoreElement)                       \
  V(StoreField)                         \
  V(StoreMessage)                       \
  V(StoreSignedSmallElement)            \
  V(StoreToObject)                      \
  V(StoreTypedElement)                  \
  V(StringCharCodeAt)                   \
  V(StringCodePointAt)                  \
  V(StringConcat)                       \
  V(StringFromCodePointAt)              \
  V(StringFromSingleCharCode)           \
  V(StringFromSingleCodePoint)          \
  V(StringIndexOf)                      \
  V(StringLength)                       \
  V(StringSubstring)                    \
  V(StringToLowerCaseIntl)              \
  V(StringToNumber)                     \
  V(StringToUpperCaseIntl)              \
  V(ToBoolean)                          \
  V(TransitionAndStoreElement)          \
  V(TransitionAndStoreNonNumberElement) \
  V(TransitionAndStoreNumberElement)    \
  V(TransitionElementsKind)             \
  V(TypeOf)                             \
  V(Unsigned32Divide)                   \
  V(VerifyType)

#define SIMPLIFIED_SPECULATIVE_BIGINT_BINOP_LIST(V) \
  V(SpeculativeBigIntAdd)                           \
  V(SpeculativeBigIntSubtract)                      \
  V(SpeculativeBigIntMultiply)                      \
  V(SpeculativeBigIntDivide)                        \
  V(SpeculativeBigIntModulus)                       \
  V(SpeculativeBigIntBitwiseAnd)                    \
  V(SpeculativeBigIntBitwiseOr)                     \
  V(SpeculativeBigIntBitwiseXor)                    \
  V(SpeculativeBigIntShiftLeft)                     \
  V(SpeculativeBigIntShiftRight)

#define SIMPLIFIED_SPECULATIVE_BIGINT_UNOP_LIST(V) \
  V(SpeculativeBigIntAsIntN)                       \
  V(SpeculativeBigIntAsUintN)                      \
  V(SpeculativeBigIntNegate)                       \
  V(SpeculativeToBigInt)

#define SIMPLIFIED_WASM_OP_LIST(V) \
  V(AssertNotNull)                 \
  V(IsNull)                        \
  V(IsNotNull)                     \
  V(Null)                          \
  V(RttCanon)                      \
  V(WasmTypeCast)                  \
  V(WasmTypeCheck)                 \
  V(WasmExternInternalize)         \
  V(WasmExternExternalize)         \
  V(WasmStructGet)                 \
  V(WasmStructSet)                 \
  V(WasmArrayGet)                  \
  V(WasmArraySet)                  \
  V(WasmArrayLength)               \
  V(WasmArrayInitializeLength)     \
  V(StringAsWtf16)                 \
  V(StringPrepareForGetCodeunit)

#define SIMPLIFIED_OP_LIST(V)                 \
  SIMPLIFIED_CHANGE_OP_LIST(V)                \
  SIMPLIFIED_CHECKED_OP_LIST(V)               \
  SIMPLIFIED_COMPARE_BINOP_LIST(V)            \
  SIMPLIFIED_NUMBER_BINOP_LIST(V)             \
  SIMPLIFIED_BIGINT_BINOP_LIST(V)             \
  SIMPLIFIED_SPECULATIVE_NUMBER_BINOP_LIST(V) \
  SIMPLIFIED_NUMBER_UNOP_LIST(V)              \
  SIMPLIFIED_BIGINT_UNOP_LIST(V)              \
  SIMPLIFIED_SPECULATIVE_NUMBER_UNOP_LIST(V)  \
  SIMPLIFIED_SPECULATIVE_BIGINT_UNOP_LIST(V)  \
  SIMPLIFIED_SPECULATIVE_BIGINT_BINOP_LIST(V) \
  IF_WASM(SIMPLIFIED_WASM_OP_LIST, V)         \
  SIMPLIFIED_OTHER_OP_LIST(V)

// Opcodes for Machine-level operators.
#define MACHINE_UNOP_32_LIST(V) \
  V(Word32Clz)                  \
  V(Word32Ctz)                  \
  V(Int32AbsWithOverflow)       \
  V(Word32ReverseBits)          \
  V(Word32ReverseBytes)

#define MACHINE_COMPARE_BINOP_LIST(V) \
  V(Word32Equal)                      \
  V(Word64Equal)                      \
  V(Int32LessThan)                    \
  V(Int32LessThanOrEqual)             \
  V(Uint32LessThan)                   \
  V(Uint32LessThanOrEqual)            \
  V(Int64LessThan)                    \
  V(Int64LessThanOrEqual)             \
  V(Uint64LessThan)                   \
  V(Uint64LessThanOrEqual)            \
  V(Float32Equal)                     \
  V(Float32LessThan)                  \
  V(Float32LessThanOrEqual)           \
  V(Float64Equal)                     \
  V(Float64LessThan)                  \
  V(Float64LessThanOrEqual)

#define MACHINE_BINOP_32_LIST(V) \
  V(Word32And)                   \
  V(Word32Or)                    \
  V(Word32Xor)                   \
  V(Word32Shl)                   \
  V(Word32Shr)                   \
  V(Word32Sar)                   \
  V(Word32Rol)                   \
  V(Word32Ror)                   \
  V(Int32Add)                    \
  V(Int32AddWithOverflow)        \
  V(Int32Sub)                    \
  V(Int32SubWithOverflow)        \
  V(Int32Mul)                    \
  V(Int32MulWithOverflow)        \
  V(Int32MulHigh)                \
  V(Int32Div)                    \
  V(Int32Mod)                    \
  V(Uint32Div)                   \
  V(Uint32Mod)                   \
  V(Uint32MulHigh)

#define MACHINE_BINOP_64_LIST(V) \
  V(Word64And)                   \
  V(Word64Or)                    \
  V(Word64Xor)                   \
  V(Word64Shl)                   \
  V(Word64Shr)                   \
  V(Word64Sar)                   \
  V(Word64Rol)                   \
  V(Word64Ror)                   \
  V(Word64RolLowerable)          \
  V(Word64RorLowerable)          \
  V(Int64Add)                    \
  V(Int64AddWithOverflow)        \
  V(Int64Sub)                    \
  V(Int64SubWithOverflow)        \
  V(Int64Mul)                    \
  V(Int64MulHigh)                \
  V(Int64MulWithOverflow)        \
  V(Int64Div)                    \
  V(Int64Mod)                    \
  V(Uint64Div)                   \
  V(Uint64Mod)                   \
  V(Uint64MulHigh)

#define MACHINE_FLOAT32_UNOP_LIST(V) \
  V(Float32Abs)                      \
  V(Float32Neg)                      \
  V(Float32RoundDown)                \
  V(Float32RoundTiesEven)            \
  V(Float32RoundTruncate)            \
  V(Float32RoundUp)                  \
  V(Float32Sqrt)

#define MACHINE_FLOAT32_BINOP_LIST(V) \
  V(Float32Add)                       \
  V(Float32Sub)                       \
  V(Float32Mul)                       \
  V(Float32Div)                       \
  V(Float32Max)                       \
  V(Float32Min)

#define MACHINE_FLOAT64_UNOP_LIST(V) \
  V(Float64Abs)                      \
  V(Float64Acos)                     \
  V(Float64Acosh)                    \
  V(Float64Asin)                     \
  V(Float64Asinh)                    \
  V(Float64Atan)                     \
  V(Float64Atanh)                    \
  V(Float64Cbrt)                     \
  V(Float64Cos)                      \
  V(Float64Cosh)                     \
  V(Float64Exp)                      \
  V(Float64Expm1)                    \
  V(Float64Log)                      \
  V(Float64Log1p)                    \
  V(Float64Log10)                    \
  V(Float64Log2)                     \
  V(Float64Neg)                      \
  V(Float64RoundDown)                \
  V(Float64RoundTiesAway)            \
  V(Float64RoundTiesEven)            \
  V(Float64RoundTruncate)            \
  V(Float64RoundUp)                  \
  V(Float64Sin)                      \
  V(Float64Sinh)                     \
  V(Float64Sqrt)                     \
  V(Float64Tan)                      \
  V(Float64Tanh)

#define MACHINE_FLOAT64_BINOP_LIST(V) \
  V(Float64Atan2)                     \
  V(Float64Max)                       \
  V(Float64Min)                       \
  V(Float64Add)                       \
  V(Float64Sub)                       \
  V(Float64Mul)                       \
  V(Float64Div)                       \
  V(Float64Mod)                       \
  V(Float64Pow)

#define MACHINE_ATOMIC_OP_LIST(V)    \
  V(Word32AtomicLoad)                \
  V(Word32AtomicStore)               \
  V(Word32AtomicExchange)            \
  V(Word32AtomicCompareExchange)     \
  V(Word32AtomicAdd)                 \
  V(Word32AtomicSub)                 \
  V(Word32AtomicAnd)                 \
  V(Word32AtomicOr)                  \
  V(Word32AtomicXor)                 \
  V(Word32AtomicPairLoad)            \
  V(Word32AtomicPairStore)           \
  V(Word32AtomicPairAdd)             \
  V(Word32AtomicPairSub)             \
  V(Word32AtomicPairAnd)             \
  V(Word32AtomicPairOr)              \
  V(Word32AtomicPairXor)             \
  V(Word32AtomicPairExchange)        \
  V(Word32AtomicPairCompareExchange) \
  V(Word64AtomicLoad)                \
  V(Word64AtomicStore)               \
  V(Word64AtomicAdd)                 \
  V(Word64AtomicSub)                 \
  V(Word64AtomicAnd)                 \
  V(Word64AtomicOr)                  \
  V(Word64AtomicXor)                 \
  V(Word64AtomicExchange)            \
  V(Word64AtomicCompareExchange)

#define MACHINE_OP_LIST(V)               \
  MACHINE_UNOP_32_LIST(V)                \
  MACHINE_BINOP_32_LIST(V)               \
  MACHINE_BINOP_64_LIST(V)               \
  MACHINE_COMPARE_BINOP_LIST(V)          \
  MACHINE_FLOAT32_BINOP_LIST(V)          \
  MACHINE_FLOAT32_UNOP_LIST(V)           \
  MACHINE_FLOAT64_BINOP_LIST(V)          \
  MACHINE_FLOAT64_UNOP_LIST(V)           \
  MACHINE_ATOMIC_OP_LIST(V)              \
  V(AbortCSADcheck)                      \
  V(DebugBreak)                          \
  V(Comment)                             \
  V(Load)                                \
  V(LoadImmutable)                       \
  V(Store)                               \
  V(StackSlot)                           \
  V(Word32Popcnt)                        \
  V(Word64Popcnt)                        \
  V(Word64Clz)                           \
  V(Word64Ctz)                           \
  V(Word64ClzLowerable)                  \
  V(Word64CtzLowerable)                  \
  V(Word64ReverseBits)                   \
  V(Word64ReverseBytes)                  \
  V(Simd128ReverseBytes)                 \
  V(Int64AbsWithOverflow)                \
  V(BitcastTaggedToWord)                 \
  V(BitcastTaggedToWordForTagAndSmiBits) \
  V(BitcastWordToTagged)                 \
  V(BitcastWordToTaggedSigned)           \
  V(TruncateFloat64ToWord32)             \
  V(ChangeFloat32ToFloat64)              \
  V(ChangeFloat64ToInt32)                \
  V(ChangeFloat64ToInt64)                \
  V(ChangeFloat64ToUint32)               \
  V(ChangeFloat64ToUint64)               \
  V(Float64SilenceNaN)                   \
  V(TruncateFloat64ToInt64)              \
  V(TruncateFloat64ToUint32)             \
  V(TruncateFloat32ToInt32)              \
  V(TruncateFloat32ToUint32)             \
  V(TryTruncateFloat32ToInt64)           \
  V(TryTruncateFloat64ToInt64)           \
  V(TryTruncateFloat32ToUint64)          \
  V(TryTruncateFloat64ToUint64)          \
  V(TryTruncateFloat64ToInt32)           \
  V(TryTruncateFloat64ToUint32)          \
  V(ChangeInt32ToFloat64)                \
  V(BitcastWord32ToWord64)               \
  V(ChangeInt32ToInt64)                  \
  V(ChangeInt64ToFloat64)                \
  V(ChangeUint32ToFloat64)               \
  V(ChangeUint32ToUint64)                \
  V(TruncateFloat64ToFloat32)            \
  V(TruncateInt64ToInt32)                \
  V(RoundFloat64ToInt32)                 \
  V(RoundInt32ToFloat32)                 \
  V(RoundInt64ToFloat32)                 \
  V(RoundInt64ToFloat64)                 \
  V(RoundUint32ToFloat32)                \
  V(RoundUint64ToFloat32)                \
  V(RoundUint64ToFloat64)                \
  V(BitcastFloat32ToInt32)               \
  V(BitcastFloat64ToInt64)               \
  V(BitcastInt32ToFloat32)               \
  V(BitcastInt64ToFloat64)               \
  V(Float64ExtractLowWord32)             \
  V(Float64ExtractHighWord32)            \
  V(Float64InsertLowWord32)              \
  V(Float64InsertHighWord32)             \
  V(Word32Select)                        \
  V(Word64Select)                        \
  V(Float32Select)                       \
  V(Float64Select)                       \
  V(LoadStackCheckOffset)                \
  V(LoadFramePointer)                    \
  V(LoadParentFramePointer)              \
  V(LoadRootRegister)                    \
  V(UnalignedLoad)                       \
  V(UnalignedStore)                      \
  V(Int32PairAdd)                        \
  V(Int32PairSub)                        \
  V(Int32PairMul)                        \
  V(Word32PairShl)                       \
  V(Word32PairShr)                       \
  V(Word32PairSar)                       \
  V(ProtectedLoad)                       \
  V(ProtectedStore)                      \
  V(LoadTrapOnNull)                      \
  V(StoreTrapOnNull)                     \
  V(MemoryBarrier)                       \
  V(SignExtendWord8ToInt32)              \
  V(SignExtendWord16ToInt32)             \
  V(SignExtendWord8ToInt64)              \
  V(SignExtendWord16ToInt64)             \
  V(SignExtendWord32ToInt64)             \
  V(StackPointerGreaterThan)             \
  V(TraceInstruction)

#define MACHINE_SIMD128_OP_LIST(V) \
  V(F64x2Splat)                    \
  V(F64x2ExtractLane)              \
  V(F64x2ReplaceLane)              \
  V(F64x2Abs)                      \
  V(F64x2Neg)                      \
  V(F64x2Sqrt)                     \
  V(F64x2Add)                      \
  V(F64x2Sub)                      \
  V(F64x2Mul)                      \
  V(F64x2Div)                      \
  V(F64x2Min)                      \
  V(F64x2Max)                      \
  V(F64x2Eq)                       \
  V(F64x2Ne)                       \
  V(F64x2Lt)                       \
  V(F64x2Le)                       \
  V(F64x2Qfma)                     \
  V(F64x2Qfms)                     \
  V(F64x2Pmin)                     \
  V(F64x2Pmax)                     \
  V(F64x2Ceil)                     \
  V(F64x2Floor)                    \
  V(F64x2Trunc)                    \
  V(F64x2NearestInt)               \
  V(F64x2ConvertLowI32x4S)         \
  V(F64x2ConvertLowI32x4U)         \
  V(F64x2PromoteLowF32x4)          \
  V(F32x4Splat)                    \
  V(F32x4ExtractLane)              \
  V(F32x4ReplaceLane)              \
  V(F32x4SConvertI32x4)            \
  V(F32x4UConvertI32x4)            \
  V(F32x4Abs)                      \
  V(F32x4Neg)                      \
  V(F32x4Sqrt)                     \
  V(F32x4Add)                      \
  V(F32x4Sub)                      \
  V(F32x4Mul)                      \
  V(F32x4Div)                      \
  V(F32x4Min)                      \
  V(F32x4Max)                      \
  V(F32x4Eq)                       \
  V(F32x4Ne)                       \
  V(F32x4Lt)                       \
  V(F32x4Le)                       \
  V(F32x4Gt)                       \
  V(F32x4Ge)                       \
  V(F32x4Qfma)                     \
  V(F32x4Qfms)                     \
  V(F32x4Pmin)                     \
  V(F32x4Pmax)                     \
  V(F32x4Ceil)                     \
  V(F32x4Floor)                    \
  V(F32x4Trunc)                    \
  V(F32x4NearestInt)               \
  V(F32x4DemoteF64x2Zero)          \
  V(I64x2Splat)                    \
  V(I64x2SplatI32Pair)             \
  V(I64x2ExtractLane)              \
  V(I64x2ReplaceLane)              \
  V(I64x2ReplaceLaneI32Pair)       \
  V(I64x2Abs)                      \
  V(I64x2Neg)                      \
  V(I64x2SConvertI32x4Low)         \
  V(I64x2SConvertI32x4High)        \
  V(I64x2UConvertI32x4Low)         \
  V(I64x2UConvertI32x4High)        \
  V(I64x2BitMask)                  \
  V(I64x2Shl)                      \
  V(I64x2ShrS)                     \
  V(I64x2Add)                      \
  V(I64x2Sub)                      \
  V(I64x2Mul)                      \
  V(I64x2Eq)                       \
  V(I64x2Ne)                       \
  V(I64x2GtS)                      \
  V(I64x2GeS)                      \
  V(I64x2ShrU)                     \
  V(I64x2ExtMulLowI32x4S)          \
  V(I64x2ExtMulHighI32x4S)         \
  V(I64x2ExtMulLowI32x4U)          \
  V(I64x2ExtMulHighI32x4U)         \
  V(I32x4Splat)                    \
  V(I32x4ExtractLane)              \
  V(I32x4ReplaceLane)              \
  V(I32x4SConvertF32x4)            \
  V(I32x4SConvertI16x8Low)         \
  V(I32x4SConvertI16x8High)        \
  V(I32x4Neg)                      \
  V(I32x4Shl)                      \
  V(I32x4ShrS)                     \
  V(I32x4Add)                      \
  V(I32x4Sub)                      \
  V(I32x4Mul)                      \
  V(I32x4MinS)                     \
  V(I32x4MaxS)                     \
  V(I32x4Eq)                       \
  V(I32x4Ne)                       \
  V(I32x4LtS)                      \
  V(I32x4LeS)                      \
  V(I32x4GtS)                      \
  V(I32x4GeS)                      \
  V(I32x4UConvertF32x4)            \
  V(I32x4UConvertI16x8Low)         \
  V(I32x4UConvertI16x8High)        \
  V(I32x4ShrU)                     \
  V(I32x4MinU)                     \
  V(I32x4MaxU)                     \
  V(I32x4LtU)                      \
  V(I32x4LeU)                      \
  V(I32x4GtU)                      \
  V(I32x4GeU)                      \
  V(I32x4Abs)                      \
  V(I32x4BitMask)                  \
  V(I32x4DotI16x8S)                \
  V(I32x4ExtMulLowI16x8S)          \
  V(I32x4ExtMulHighI16x8S)         \
  V(I32x4ExtMulLowI16x8U)          \
  V(I32x4ExtMulHighI16x8U)         \
  V(I32x4ExtAddPairwiseI16x8S)     \
  V(I32x4ExtAddPairwiseI16x8U)     \
  V(I32x4TruncSatF64x2SZero)       \
  V(I32x4TruncSatF64x2UZero)       \
  V(I16x8Splat)                    \
  V(I16x8ExtractLaneU)             \
  V(I16x8ExtractLaneS)             \
  V(I16x8ReplaceLane)              \
  V(I16x8SConvertI8x16Low)         \
  V(I16x8SConvertI8x16High)        \
  V(I16x8Neg)                      \
  V(I16x8Shl)                      \
  V(I16x8ShrS)                     \
  V(I16x8SConvertI32x4)            \
  V(I16x8Add)                      \
  V(I16x8AddSatS)                  \
  V(I16x8Sub)                      \
  V(I16x8SubSatS)                  \
  V(I16x8Mul)                      \
  V(I16x8MinS)                     \
  V(I16x8MaxS)                     \
  V(I16x8Eq)                       \
  V(I16x8Ne)                       \
  V(I16x8LtS)                      \
  V(I16x8LeS)                      \
  V(I16x8GtS)                      \
  V(I16x8GeS)                      \
  V(I16x8UConvertI8x16Low)         \
  V(I16x8UConvertI8x16High)        \
  V(I16x8ShrU)                     \
  V(I16x8UConvertI32x4)            \
  V(I16x8AddSatU)                  \
  V(I16x8SubSatU)                  \
  V(I16x8MinU)                     \
  V(I16x8MaxU)                     \
  V(I16x8LtU)                      \
  V(I16x8LeU)                      \
  V(I16x8GtU)                      \
  V(I16x8GeU)                      \
  V(I16x8RoundingAverageU)         \
  V(I16x8Q15MulRSatS)              \
  V(I16x8Abs)                      \
  V(I16x8BitMask)                  \
  V(I16x8ExtMulLowI8x16S)          \
  V(I16x8ExtMulHighI8x16S)         \
  V(I16x8ExtMulLowI8x16U)          \
  V(I16x8ExtMulHighI8x16U)         \
  V(I16x8ExtAddPairwiseI8x16S)     \
  V(I16x8ExtAddPairwiseI8x16U)     \
  V(I8x16Splat)                    \
  V(I8x16ExtractLaneU)             \
  V(I8x16ExtractLaneS)             \
  V(I8x16ReplaceLane)              \
  V(I8x16SConvertI16x8)            \
  V(I8x16Neg)                      \
  V(I8x16Shl)                      \
  V(I8x16ShrS)                     \
  V(I8x16Add)                      \
  V(I8x16AddSatS)                  \
  V(I8x16Sub)                      \
  V(I8x16SubSatS)                  \
  V(I8x16MinS)                     \
  V(I8x16MaxS)                     \
  V(I8x16Eq)                       \
  V(I8x16Ne)                       \
  V(I8x16LtS)                      \
  V(I8x16LeS)                      \
  V(I8x16GtS)                      \
  V(I8x16GeS)                      \
  V(I8x16UConvertI16x8)            \
  V(I8x16AddSatU)                  \
  V(I8x16SubSatU)                  \
  V(I8x16ShrU)                     \
  V(I8x16MinU)                     \
  V(I8x16MaxU)                     \
  V(I8x16LtU)                      \
  V(I8x16LeU)                      \
  V(I8x16GtU)                      \
  V(I8x16GeU)                      \
  V(I8x16RoundingAverageU)         \
  V(I8x16Popcnt)                   \
  V(I8x16Abs)                      \
  V(I8x16BitMask)                  \
  V(S128Zero)                      \
  V(S128Const)                     \
  V(S128Not)                       \
  V(S128And)                       \
  V(S128Or)                        \
  V(S128Xor)                       \
  V(S128Select)                    \
  V(S128AndNot)                    \
  V(I8x16Swizzle)                  \
  V(I8x16RelaxedLaneSelect)        \
  V(I16x8RelaxedLaneSelect)        \
  V(I32x4RelaxedLaneSelect)        \
  V(I64x2RelaxedLaneSelect)        \
  V(F32x4RelaxedMin)               \
  V(F32x4RelaxedMax)               \
  V(F64x2RelaxedMin)               \
  V(F64x2RelaxedMax)               \
  V(I32x4RelaxedTruncF32x4S)       \
  V(I32x4RelaxedTruncF32x4U)       \
  V(I32x4RelaxedTruncF64x2SZero)   \
  V(I32x4RelaxedTruncF64x2UZero)   \
  V(I16x8RelaxedQ15MulRS)          \
  V(I16x8DotI8x16I7x16S)           \
  V(I32x4DotI8x16I7x16AddS)        \
  V(I8x16Shuffle)                  \
  V(V128AnyTrue)                   \
  V(I64x2AllTrue)                  \
  V(I32x4AllTrue)                  \
  V(I16x8AllTrue)                  \
  V(I8x16AllTrue)                  \
  V(LoadTransform)                 \
  V(LoadLane)                      \
  V(StoreLane)

// SIMD256 for AVX
#define MACHINE_SIMD256_OP_LIST(V) \
  V(F64x4Add)                      \
  V(F32x8Add)                      \
  V(I64x4Add)                      \
  V(I32x8Add)                      \
  V(I16x16Add)                     \
  V(I8x32Add)                      \
  V(F64x4Sub)                      \
  V(F32x8Sub)                      \
  V(I64x4Sub)                      \
  V(I32x8Sub)                      \
  V(I16x16Sub)                     \
  V(I8x32Sub)                      \
  V(F64x4Mul)                      \
  V(F32x8Mul)                      \
  V(I64x4Mul)                      \
  V(I32x8Mul)                      \
  V(I16x16Mul)                     \
  V(F64x4Div)                      \
  V(F32x8Div)                      \
  V(I16x16AddSatS)                 \
  V(I8x32AddSatS)                  \
  V(I16x16AddSatU)                 \
  V(I8x32AddSatU)                  \
  V(I16x16SubSatS)                 \
  V(I8x32SubSatS)                  \
  V(I16x16SubSatU)                 \
  V(I8x32SubSatU)                  \
  V(F32x8Pmin)                     \
  V(F32x8Pmax)                     \
  V(F32x8Eq)                       \
  V(F32x8Ne)                       \
  V(F32x8Lt)                       \
  V(F32x8Le)                       \
  V(S256Select)                    \
  V(ExtractF128)

#define VALUE_OP_LIST(V)     \
  COMMON_OP_LIST(V)          \
  SIMPLIFIED_OP_LIST(V)      \
  MACHINE_OP_LIST(V)         \
  MACHINE_SIMD128_OP_LIST(V) \
  MACHINE_SIMD256_OP_LIST(V) \
  JS_OP_LIST(V)

// The combination of all operators at all levels and the common operators.
#define ALL_OP_LIST(V) \
  CONTROL_OP_LIST(V)   \
  VALUE_OP_LIST(V)

namespace v8 {
namespace internal {
namespace compiler {

// Declare an enumeration with all the opcodes at all levels so that they
// can be globally, uniquely numbered.
class V8_EXPORT_PRIVATE IrOpcode {
 public:
  enum Value {
#define DECLARE_OPCODE(x, ...) k##x,
    ALL_OP_LIST(DECLARE_OPCODE)
#undef DECLARE_OPCODE
        kLast = -1
#define COUNT_OPCODE(...) +1
                ALL_OP_LIST(COUNT_OPCODE)
#undef COUNT_OPCODE
  };

  // Returns the mnemonic name of an opcode.
  static char const* Mnemonic(Value value);

  // Returns true if opcode for common operator.
  static bool IsCommonOpcode(Value value) {
    return kStart <= value && value <= kStaticAssert;
  }

  // Returns true if opcode for control operator.
  static bool IsControlOpcode(Value value) {
    return kStart <= value && value <= kEnd;
  }

  // Returns true if opcode for JavaScript operator.
  static bool IsJsOpcode(Value value) {
    return kJSEqual <= value && value <= kJSDebugger;
  }

  // Returns true if opcode for machine operator.
  static bool IsMachineOpcode(Value value) {
    return kWord32Clz <= value && value <= kTraceInstruction;
  }

  // Returns true iff opcode is a machine-level constant.
  static bool IsMachineConstantOpcode(Value value) {
    switch (value) {
#define CASE(name) \
  case k##name:    \
    return true;
      MACHINE_LEVEL_CONSTANT_OP_LIST(CASE)
#undef CASE
      default:
        return false;
    }
  }

  // Returns true if opcode for constant operator.
  static bool IsConstantOpcode(Value value) {
#define CASE(Name) \
  case k##Name:    \
    return true;
    switch (value) {
      CONSTANT_OP_LIST(CASE);
      default:
        return false;
    }
#undef CASE
    UNREACHABLE();
  }

  static bool IsPhiOpcode(Value value) {
    return value == kPhi || value == kEffectPhi;
  }

  static bool IsMergeOpcode(Value value) {
    return value == kMerge || value == kLoop;
  }

  static bool IsIfProjectionOpcode(Value value) {
    return kIfTrue <= value && value <= kIfDefault;
  }

  // Returns true if opcode terminates control flow in a graph (i.e.
  // respective nodes are expected to have control uses by the graphs {End}
  // node only).
  static bool IsGraphTerminator(Value value) {
    return value == kDeoptimize || value == kReturn || value == kTailCall ||
           value == kTerminate || value == kThrow;
  }

  // Returns true if opcode can be inlined.
  static bool IsInlineeOpcode(Value value) {
    return value == kJSConstruct || value == kJSCall;
  }

  // Returns true if opcode for comparison operator.
  static bool IsComparisonOpcode(Value value) {
#define CASE(Name, ...) \
  case k##Name:         \
    return true;
    switch (value) {
      JS_COMPARE_BINOP_LIST(CASE);
      SIMPLIFIED_COMPARE_BINOP_LIST(CASE);
      MACHINE_COMPARE_BINOP_LIST(CASE);
      default:
        return false;
    }
#undef CASE
    UNREACHABLE();
  }

  static bool IsContextChainExtendingOpcode(Value value) {
    return kJSCreateFunctionContext <= value && value <= kJSCreateBlockContext;
  }

  // These opcode take the feedback vector as an input, and implement
  // feedback-collecting logic in generic lowering.
  static bool IsFeedbackCollectingOpcode(Value value) {
#define CASE(Name, ...) \
  case k##Name:         \
    return true;
    switch (value) {
      JS_ARITH_BINOP_LIST(CASE)
      JS_ARITH_UNOP_LIST(CASE)
      JS_BITWISE_BINOP_LIST(CASE)
      JS_BITWISE_UNOP_LIST(CASE)
      JS_COMPARE_BINOP_LIST(CASE)
      case kJSCall:
      case kJSCallWithArrayLike:
      case kJSCallWithSpread:
      case kJSCloneObject:
      case kJSConstruct:
      case kJSConstructWithArrayLike:
      case kJSConstructWithSpread:
      case kJSCreateEmptyLiteralArray:
      case kJSCreateLiteralArray:
      case kJSCreateLiteralObject:
      case kJSCreateLiteralRegExp:
      case kJSDefineKeyedOwnProperty:
      case kJSForInNext:
      case kJSForInPrepare:
      case kJSGetIterator:
      case kJSGetTemplateObject:
      case kJSHasProperty:
      case kJSInstanceOf:
      case kJSLoadGlobal:
      case kJSLoadNamed:
      case kJSLoadNamedFromSuper:
      case kJSLoadProperty:
      case kJSDefineKeyedOwnPropertyInLiteral:
      case kJSStoreGlobal:
      case kJSStoreInArrayLiteral:
      case kJSSetNamedProperty:
      case kJSDefineNamedOwnProperty:
      case kJSSetKeyedProperty:
        return true;
      default:
        return false;
    }
#undef CASE
    UNREACHABLE();
  }

  static bool IsFeedbackCollectingOpcode(int16_t value) {
    DCHECK(0 <= value && value <= kLast);
    return IsFeedbackCollectingOpcode(static_cast<IrOpcode::Value>(value));
  }

  static bool isAtomicOpOpcode(Value value) {
    switch (value) {
    #define CASE(Name, ...) \
      case k##Name:         \
        return true;
      MACHINE_ATOMIC_OP_LIST(CASE)
      default:
        return false;
    #undef CASE
    }
    UNREACHABLE();
  }

  static bool IsSimd128Opcode(Value value) {
#define CASE(Name, ...) case k##Name:
    switch (value) {
      MACHINE_SIMD128_OP_LIST(CASE)
      return true;
      default:
        return false;
    }
#undef CASE
    UNREACHABLE();
  }
};

V8_EXPORT_PRIVATE std::ostream& operator<<(std::ostream&, IrOpcode::Value);

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_OPCODES_H_

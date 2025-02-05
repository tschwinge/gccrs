// Copyright (C) 2020-2022 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#ifndef RUST_COMPILE_BLOCK
#define RUST_COMPILE_BLOCK

#include "rust-compile-base.h"
#include "rust-compile-tyty.h"

namespace Rust {
namespace Compile {

class CompileBlock : public HIRCompileBase
{
  using Rust::Compile::HIRCompileBase::visit;

public:
  static tree compile (HIR::BlockExpr *expr, Context *ctx, Bvariable *result)
  {
    CompileBlock compiler (ctx, result);
    expr->accept_vis (compiler);
    return compiler.translated;
  }

  void visit (HIR::BlockExpr &expr) override;

private:
  CompileBlock (Context *ctx, Bvariable *result)
    : HIRCompileBase (ctx), translated (nullptr), result (result)
  {}

  tree translated;
  Bvariable *result;
};

class CompileConditionalBlocks : public HIRCompileBase
{
  using Rust::Compile::HIRCompileBase::visit;

public:
  static tree compile (HIR::IfExpr *expr, Context *ctx, Bvariable *result)
  {
    CompileConditionalBlocks resolver (ctx, result);
    expr->accept_vis (resolver);
    return resolver.translated;
  }

  void visit (HIR::IfExpr &expr) override;

  void visit (HIR::IfExprConseqElse &expr) override;

  void visit (HIR::IfExprConseqIf &expr) override;

private:
  CompileConditionalBlocks (Context *ctx, Bvariable *result)
    : HIRCompileBase (ctx), translated (nullptr), result (result)
  {}

  tree translated;
  Bvariable *result;
};

class CompileExprWithBlock : public HIRCompileBase
{
  using Rust::Compile::HIRCompileBase::visit;

public:
  static tree compile (HIR::ExprWithBlock *expr, Context *ctx,
		       Bvariable *result)
  {
    CompileExprWithBlock resolver (ctx, result);
    expr->accept_vis (resolver);
    return resolver.translated;
  }

  void visit (HIR::IfExpr &expr) override
  {
    translated = CompileConditionalBlocks::compile (&expr, ctx, result);
  }

  void visit (HIR::IfExprConseqElse &expr) override
  {
    translated = CompileConditionalBlocks::compile (&expr, ctx, result);
  }

  void visit (HIR::IfExprConseqIf &expr) override
  {
    translated = CompileConditionalBlocks::compile (&expr, ctx, result);
  }

private:
  CompileExprWithBlock (Context *ctx, Bvariable *result)
    : HIRCompileBase (ctx), translated (nullptr), result (result)
  {}

  tree translated;
  Bvariable *result;
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_BLOCK

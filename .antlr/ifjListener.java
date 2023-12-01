// Generated from /home/ramsay/school/ifj/ifj-project/ifj.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link ifjParser}.
 */
public interface ifjListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link ifjParser#prog}.
	 * @param ctx the parse tree
	 */
	void enterProg(ifjParser.ProgContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#prog}.
	 * @param ctx the parse tree
	 */
	void exitProg(ifjParser.ProgContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#return_def}.
	 * @param ctx the parse tree
	 */
	void enterReturn_def(ifjParser.Return_defContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#return_def}.
	 * @param ctx the parse tree
	 */
	void exitReturn_def(ifjParser.Return_defContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#func_params}.
	 * @param ctx the parse tree
	 */
	void enterFunc_params(ifjParser.Func_paramsContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#func_params}.
	 * @param ctx the parse tree
	 */
	void exitFunc_params(ifjParser.Func_paramsContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#func_params_n}.
	 * @param ctx the parse tree
	 */
	void enterFunc_params_n(ifjParser.Func_params_nContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#func_params_n}.
	 * @param ctx the parse tree
	 */
	void exitFunc_params_n(ifjParser.Func_params_nContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#statement_list}.
	 * @param ctx the parse tree
	 */
	void enterStatement_list(ifjParser.Statement_listContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#statement_list}.
	 * @param ctx the parse tree
	 */
	void exitStatement_list(ifjParser.Statement_listContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#statement}.
	 * @param ctx the parse tree
	 */
	void enterStatement(ifjParser.StatementContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#statement}.
	 * @param ctx the parse tree
	 */
	void exitStatement(ifjParser.StatementContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#var_definition_value}.
	 * @param ctx the parse tree
	 */
	void enterVar_definition_value(ifjParser.Var_definition_valueContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#var_definition_value}.
	 * @param ctx the parse tree
	 */
	void exitVar_definition_value(ifjParser.Var_definition_valueContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#optional_expr}.
	 * @param ctx the parse tree
	 */
	void enterOptional_expr(ifjParser.Optional_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#optional_expr}.
	 * @param ctx the parse tree
	 */
	void exitOptional_expr(ifjParser.Optional_exprContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterExpr(ifjParser.ExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitExpr(ifjParser.ExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#call_params}.
	 * @param ctx the parse tree
	 */
	void enterCall_params(ifjParser.Call_paramsContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#call_params}.
	 * @param ctx the parse tree
	 */
	void exitCall_params(ifjParser.Call_paramsContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#call_param_n}.
	 * @param ctx the parse tree
	 */
	void enterCall_param_n(ifjParser.Call_param_nContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#call_param_n}.
	 * @param ctx the parse tree
	 */
	void exitCall_param_n(ifjParser.Call_param_nContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#call_param_kw}.
	 * @param ctx the parse tree
	 */
	void enterCall_param_kw(ifjParser.Call_param_kwContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#call_param_kw}.
	 * @param ctx the parse tree
	 */
	void exitCall_param_kw(ifjParser.Call_param_kwContext ctx);
	/**
	 * Enter a parse tree produced by {@link ifjParser#term}.
	 * @param ctx the parse tree
	 */
	void enterTerm(ifjParser.TermContext ctx);
	/**
	 * Exit a parse tree produced by {@link ifjParser#term}.
	 * @param ctx the parse tree
	 */
	void exitTerm(ifjParser.TermContext ctx);
}
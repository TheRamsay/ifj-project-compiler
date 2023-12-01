// Generated from ./ifj.g4 by ANTLR 4.7.2
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class ifjParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.7.2", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, LITERAL=17, 
		VAR_DEFINITION_KW=18, TYPE=19, FUNC_ID=20, PARAM_OUT_ID=21, PARAM_IN_ID=22, 
		IDENTIFIER=23, INTEGER_LITERAL=24, STRING_LITERAL=25;
	public static final int
		RULE_prog = 0, RULE_return_def = 1, RULE_func_params = 2, RULE_func_params_n = 3, 
		RULE_statement_list = 4, RULE_statement = 5, RULE_var_definition_value = 6, 
		RULE_optional_expr = 7, RULE_expr = 8, RULE_call_params = 9, RULE_call_param_n = 10, 
		RULE_call_param_kw = 11, RULE_term = 12;
	private static String[] makeRuleNames() {
		return new String[] {
			"prog", "return_def", "func_params", "func_params_n", "statement_list", 
			"statement", "var_definition_value", "optional_expr", "expr", "call_params", 
			"call_param_n", "call_param_kw", "term"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'func'", "'('", "')'", "'{'", "'}'", "'->'", "':'", "','", "'if ('", 
			"') {'", "'} else {'", "'while ('", "'break'", "'continue'", "'return'", 
			"'='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, "LITERAL", "VAR_DEFINITION_KW", "TYPE", 
			"FUNC_ID", "PARAM_OUT_ID", "PARAM_IN_ID", "IDENTIFIER", "INTEGER_LITERAL", 
			"STRING_LITERAL"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "ifj.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public ifjParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}
	public static class ProgContext extends ParserRuleContext {
		public TerminalNode FUNC_ID() { return getToken(ifjParser.FUNC_ID, 0); }
		public Func_paramsContext func_params() {
			return getRuleContext(Func_paramsContext.class,0);
		}
		public Return_defContext return_def() {
			return getRuleContext(Return_defContext.class,0);
		}
		public Statement_listContext statement_list() {
			return getRuleContext(Statement_listContext.class,0);
		}
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterProg(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitProg(this);
		}
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_prog);
		try {
			setState(38);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,0,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(26);
				match(T__0);
				setState(27);
				match(FUNC_ID);
				setState(28);
				match(T__1);
				setState(29);
				func_params();
				setState(30);
				match(T__2);
				setState(31);
				return_def();
				setState(32);
				match(T__3);
				setState(33);
				statement_list();
				setState(34);
				match(T__4);
				setState(35);
				prog();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Return_defContext extends ParserRuleContext {
		public TerminalNode TYPE() { return getToken(ifjParser.TYPE, 0); }
		public Return_defContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_return_def; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterReturn_def(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitReturn_def(this);
		}
	}

	public final Return_defContext return_def() throws RecognitionException {
		Return_defContext _localctx = new Return_defContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_return_def);
		try {
			setState(43);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__5:
				enterOuterAlt(_localctx, 1);
				{
				setState(40);
				match(T__5);
				setState(41);
				match(TYPE);
				}
				break;
			case T__3:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Func_paramsContext extends ParserRuleContext {
		public TerminalNode PARAM_OUT_ID() { return getToken(ifjParser.PARAM_OUT_ID, 0); }
		public TerminalNode PARAM_IN_ID() { return getToken(ifjParser.PARAM_IN_ID, 0); }
		public TerminalNode TYPE() { return getToken(ifjParser.TYPE, 0); }
		public Func_params_nContext func_params_n() {
			return getRuleContext(Func_params_nContext.class,0);
		}
		public Func_paramsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_func_params; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterFunc_params(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitFunc_params(this);
		}
	}

	public final Func_paramsContext func_params() throws RecognitionException {
		Func_paramsContext _localctx = new Func_paramsContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_func_params);
		try {
			setState(51);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PARAM_OUT_ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(45);
				match(PARAM_OUT_ID);
				setState(46);
				match(PARAM_IN_ID);
				setState(47);
				match(T__6);
				setState(48);
				match(TYPE);
				setState(49);
				func_params_n();
				}
				break;
			case T__2:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Func_params_nContext extends ParserRuleContext {
		public TerminalNode PARAM_OUT_ID() { return getToken(ifjParser.PARAM_OUT_ID, 0); }
		public TerminalNode PARAM_IN_ID() { return getToken(ifjParser.PARAM_IN_ID, 0); }
		public TerminalNode TYPE() { return getToken(ifjParser.TYPE, 0); }
		public Func_params_nContext func_params_n() {
			return getRuleContext(Func_params_nContext.class,0);
		}
		public Func_params_nContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_func_params_n; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterFunc_params_n(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitFunc_params_n(this);
		}
	}

	public final Func_params_nContext func_params_n() throws RecognitionException {
		Func_params_nContext _localctx = new Func_params_nContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_func_params_n);
		try {
			setState(60);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__7:
				enterOuterAlt(_localctx, 1);
				{
				setState(53);
				match(T__7);
				setState(54);
				match(PARAM_OUT_ID);
				setState(55);
				match(PARAM_IN_ID);
				setState(56);
				match(T__6);
				setState(57);
				match(TYPE);
				setState(58);
				func_params_n();
				}
				break;
			case T__2:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Statement_listContext extends ParserRuleContext {
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public Statement_listContext statement_list() {
			return getRuleContext(Statement_listContext.class,0);
		}
		public Statement_listContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement_list; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterStatement_list(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitStatement_list(this);
		}
	}

	public final Statement_listContext statement_list() throws RecognitionException {
		Statement_listContext _localctx = new Statement_listContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_statement_list);
		try {
			setState(66);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__8:
			case T__11:
			case T__12:
			case T__13:
			case T__14:
			case LITERAL:
			case VAR_DEFINITION_KW:
			case FUNC_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(62);
				statement();
				setState(63);
				statement_list();
				}
				break;
			case T__4:
			case T__10:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StatementContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public List<Statement_listContext> statement_list() {
			return getRuleContexts(Statement_listContext.class);
		}
		public Statement_listContext statement_list(int i) {
			return getRuleContext(Statement_listContext.class,i);
		}
		public Optional_exprContext optional_expr() {
			return getRuleContext(Optional_exprContext.class,0);
		}
		public TerminalNode FUNC_ID() { return getToken(ifjParser.FUNC_ID, 0); }
		public Call_paramsContext call_params() {
			return getRuleContext(Call_paramsContext.class,0);
		}
		public TerminalNode VAR_DEFINITION_KW() { return getToken(ifjParser.VAR_DEFINITION_KW, 0); }
		public TerminalNode IDENTIFIER() { return getToken(ifjParser.IDENTIFIER, 0); }
		public Var_definition_valueContext var_definition_value() {
			return getRuleContext(Var_definition_valueContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterStatement(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitStatement(this);
		}
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_statement);
		try {
			setState(95);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,5,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(68);
				match(T__8);
				setState(69);
				expr();
				setState(70);
				match(T__9);
				setState(71);
				statement_list();
				setState(72);
				match(T__10);
				setState(73);
				statement_list();
				setState(74);
				match(T__4);
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(76);
				match(T__11);
				setState(77);
				expr();
				setState(78);
				match(T__9);
				setState(79);
				statement_list();
				setState(80);
				match(T__4);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(82);
				match(T__12);
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(83);
				match(T__13);
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(84);
				match(T__14);
				setState(85);
				optional_expr();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(86);
				match(FUNC_ID);
				setState(87);
				match(T__1);
				setState(88);
				call_params();
				setState(89);
				match(T__2);
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(91);
				match(VAR_DEFINITION_KW);
				setState(92);
				match(IDENTIFIER);
				setState(93);
				var_definition_value();
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(94);
				expr();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Var_definition_valueContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public Var_definition_valueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_var_definition_value; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterVar_definition_value(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitVar_definition_value(this);
		}
	}

	public final Var_definition_valueContext var_definition_value() throws RecognitionException {
		Var_definition_valueContext _localctx = new Var_definition_valueContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_var_definition_value);
		try {
			setState(100);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__15:
				enterOuterAlt(_localctx, 1);
				{
				setState(97);
				match(T__15);
				setState(98);
				expr();
				}
				break;
			case T__4:
			case T__8:
			case T__10:
			case T__11:
			case T__12:
			case T__13:
			case T__14:
			case LITERAL:
			case VAR_DEFINITION_KW:
			case FUNC_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Optional_exprContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public Optional_exprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_optional_expr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterOptional_expr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitOptional_expr(this);
		}
	}

	public final Optional_exprContext optional_expr() throws RecognitionException {
		Optional_exprContext _localctx = new Optional_exprContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_optional_expr);
		try {
			setState(104);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(102);
				expr();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExprContext extends ParserRuleContext {
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public TerminalNode FUNC_ID() { return getToken(ifjParser.FUNC_ID, 0); }
		public Call_paramsContext call_params() {
			return getRuleContext(Call_paramsContext.class,0);
		}
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterExpr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitExpr(this);
		}
	}

	public final ExprContext expr() throws RecognitionException {
		ExprContext _localctx = new ExprContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_expr);
		try {
			setState(112);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LITERAL:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(106);
				term();
				}
				break;
			case FUNC_ID:
				enterOuterAlt(_localctx, 2);
				{
				setState(107);
				match(FUNC_ID);
				setState(108);
				match(T__1);
				setState(109);
				call_params();
				setState(110);
				match(T__2);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Call_paramsContext extends ParserRuleContext {
		public Call_param_kwContext call_param_kw() {
			return getRuleContext(Call_param_kwContext.class,0);
		}
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public Call_param_nContext call_param_n() {
			return getRuleContext(Call_param_nContext.class,0);
		}
		public Call_paramsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_call_params; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterCall_params(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitCall_params(this);
		}
	}

	public final Call_paramsContext call_params() throws RecognitionException {
		Call_paramsContext _localctx = new Call_paramsContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_call_params);
		try {
			setState(119);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LITERAL:
			case PARAM_OUT_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(114);
				call_param_kw();
				setState(115);
				term();
				setState(116);
				call_param_n();
				}
				break;
			case T__2:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Call_param_nContext extends ParserRuleContext {
		public Call_param_kwContext call_param_kw() {
			return getRuleContext(Call_param_kwContext.class,0);
		}
		public TermContext term() {
			return getRuleContext(TermContext.class,0);
		}
		public Call_param_nContext call_param_n() {
			return getRuleContext(Call_param_nContext.class,0);
		}
		public Call_param_nContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_call_param_n; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterCall_param_n(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitCall_param_n(this);
		}
	}

	public final Call_param_nContext call_param_n() throws RecognitionException {
		Call_param_nContext _localctx = new Call_param_nContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_call_param_n);
		try {
			setState(126);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LITERAL:
			case PARAM_OUT_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(121);
				call_param_kw();
				setState(122);
				term();
				setState(123);
				call_param_n();
				}
				break;
			case T__2:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Call_param_kwContext extends ParserRuleContext {
		public TerminalNode PARAM_OUT_ID() { return getToken(ifjParser.PARAM_OUT_ID, 0); }
		public Call_param_kwContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_call_param_kw; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterCall_param_kw(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitCall_param_kw(this);
		}
	}

	public final Call_param_kwContext call_param_kw() throws RecognitionException {
		Call_param_kwContext _localctx = new Call_param_kwContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_call_param_kw);
		try {
			setState(131);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PARAM_OUT_ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(128);
				match(PARAM_OUT_ID);
				setState(129);
				match(T__6);
				}
				break;
			case LITERAL:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 2);
				{
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TermContext extends ParserRuleContext {
		public TerminalNode IDENTIFIER() { return getToken(ifjParser.IDENTIFIER, 0); }
		public TerminalNode LITERAL() { return getToken(ifjParser.LITERAL, 0); }
		public TermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_term; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).enterTerm(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof ifjListener ) ((ifjListener)listener).exitTerm(this);
		}
	}

	public final TermContext term() throws RecognitionException {
		TermContext _localctx = new TermContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_term);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(133);
			_la = _input.LA(1);
			if ( !(_la==LITERAL || _la==IDENTIFIER) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3\33\u008a\4\2\t\2"+
		"\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\3\2\3\2\3\2\3\2\3\2\3\2\3\2\3\2\3\2\3"+
		"\2\3\2\3\2\5\2)\n\2\3\3\3\3\3\3\5\3.\n\3\3\4\3\4\3\4\3\4\3\4\3\4\5\4\66"+
		"\n\4\3\5\3\5\3\5\3\5\3\5\3\5\3\5\5\5?\n\5\3\6\3\6\3\6\3\6\5\6E\n\6\3\7"+
		"\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3"+
		"\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3\7\5\7b\n\7\3\b\3\b\3\b\5\bg\n\b\3\t\3"+
		"\t\5\tk\n\t\3\n\3\n\3\n\3\n\3\n\3\n\5\ns\n\n\3\13\3\13\3\13\3\13\3\13"+
		"\5\13z\n\13\3\f\3\f\3\f\3\f\3\f\5\f\u0081\n\f\3\r\3\r\3\r\5\r\u0086\n"+
		"\r\3\16\3\16\3\16\2\2\17\2\4\6\b\n\f\16\20\22\24\26\30\32\2\3\4\2\23\23"+
		"\31\31\2\u008e\2(\3\2\2\2\4-\3\2\2\2\6\65\3\2\2\2\b>\3\2\2\2\nD\3\2\2"+
		"\2\fa\3\2\2\2\16f\3\2\2\2\20j\3\2\2\2\22r\3\2\2\2\24y\3\2\2\2\26\u0080"+
		"\3\2\2\2\30\u0085\3\2\2\2\32\u0087\3\2\2\2\34\35\7\3\2\2\35\36\7\26\2"+
		"\2\36\37\7\4\2\2\37 \5\6\4\2 !\7\5\2\2!\"\5\4\3\2\"#\7\6\2\2#$\5\n\6\2"+
		"$%\7\7\2\2%&\5\2\2\2&)\3\2\2\2\')\3\2\2\2(\34\3\2\2\2(\'\3\2\2\2)\3\3"+
		"\2\2\2*+\7\b\2\2+.\7\25\2\2,.\3\2\2\2-*\3\2\2\2-,\3\2\2\2.\5\3\2\2\2/"+
		"\60\7\27\2\2\60\61\7\30\2\2\61\62\7\t\2\2\62\63\7\25\2\2\63\66\5\b\5\2"+
		"\64\66\3\2\2\2\65/\3\2\2\2\65\64\3\2\2\2\66\7\3\2\2\2\678\7\n\2\289\7"+
		"\27\2\29:\7\30\2\2:;\7\t\2\2;<\7\25\2\2<?\5\b\5\2=?\3\2\2\2>\67\3\2\2"+
		"\2>=\3\2\2\2?\t\3\2\2\2@A\5\f\7\2AB\5\n\6\2BE\3\2\2\2CE\3\2\2\2D@\3\2"+
		"\2\2DC\3\2\2\2E\13\3\2\2\2FG\7\13\2\2GH\5\22\n\2HI\7\f\2\2IJ\5\n\6\2J"+
		"K\7\r\2\2KL\5\n\6\2LM\7\7\2\2Mb\3\2\2\2NO\7\16\2\2OP\5\22\n\2PQ\7\f\2"+
		"\2QR\5\n\6\2RS\7\7\2\2Sb\3\2\2\2Tb\7\17\2\2Ub\7\20\2\2VW\7\21\2\2Wb\5"+
		"\20\t\2XY\7\26\2\2YZ\7\4\2\2Z[\5\24\13\2[\\\7\5\2\2\\b\3\2\2\2]^\7\24"+
		"\2\2^_\7\31\2\2_b\5\16\b\2`b\5\22\n\2aF\3\2\2\2aN\3\2\2\2aT\3\2\2\2aU"+
		"\3\2\2\2aV\3\2\2\2aX\3\2\2\2a]\3\2\2\2a`\3\2\2\2b\r\3\2\2\2cd\7\22\2\2"+
		"dg\5\22\n\2eg\3\2\2\2fc\3\2\2\2fe\3\2\2\2g\17\3\2\2\2hk\5\22\n\2ik\3\2"+
		"\2\2jh\3\2\2\2ji\3\2\2\2k\21\3\2\2\2ls\5\32\16\2mn\7\26\2\2no\7\4\2\2"+
		"op\5\24\13\2pq\7\5\2\2qs\3\2\2\2rl\3\2\2\2rm\3\2\2\2s\23\3\2\2\2tu\5\30"+
		"\r\2uv\5\32\16\2vw\5\26\f\2wz\3\2\2\2xz\3\2\2\2yt\3\2\2\2yx\3\2\2\2z\25"+
		"\3\2\2\2{|\5\30\r\2|}\5\32\16\2}~\5\26\f\2~\u0081\3\2\2\2\177\u0081\3"+
		"\2\2\2\u0080{\3\2\2\2\u0080\177\3\2\2\2\u0081\27\3\2\2\2\u0082\u0083\7"+
		"\27\2\2\u0083\u0086\7\t\2\2\u0084\u0086\3\2\2\2\u0085\u0082\3\2\2\2\u0085"+
		"\u0084\3\2\2\2\u0086\31\3\2\2\2\u0087\u0088\t\2\2\2\u0088\33\3\2\2\2\16"+
		"(-\65>Dafjry\u0080\u0085";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}
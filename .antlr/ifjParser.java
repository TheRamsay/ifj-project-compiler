// Generated from /home/ramsay/school/ifj/ifj-project/ifj.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class ifjParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, LITERAL=17, 
		VAR_DEFINITION=18, VAR_DEFINITION_KW=19, TYPE=20, FUNC_ID=21, PARAM_OUT_ID=22, 
		PARAM_IN_ID=23, IDENTIFIER=24, INTEGER_LITERAL=25, STRING_LITERAL=26;
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
			null, null, null, null, null, "LITERAL", "VAR_DEFINITION", "VAR_DEFINITION_KW", 
			"TYPE", "FUNC_ID", "PARAM_OUT_ID", "PARAM_IN_ID", "IDENTIFIER", "INTEGER_LITERAL", 
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

	@SuppressWarnings("CheckReturnValue")
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
		public StatementContext statement() {
			return getRuleContext(StatementContext.class,0);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_prog);
		try {
			setState(41);
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
				setState(37);
				statement();
				setState(38);
				prog();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
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

	@SuppressWarnings("CheckReturnValue")
	public static class Return_defContext extends ParserRuleContext {
		public TerminalNode TYPE() { return getToken(ifjParser.TYPE, 0); }
		public Return_defContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_return_def; }
	}

	public final Return_defContext return_def() throws RecognitionException {
		Return_defContext _localctx = new Return_defContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_return_def);
		try {
			setState(46);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__5:
				enterOuterAlt(_localctx, 1);
				{
				setState(43);
				match(T__5);
				setState(44);
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

	@SuppressWarnings("CheckReturnValue")
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
	}

	public final Func_paramsContext func_params() throws RecognitionException {
		Func_paramsContext _localctx = new Func_paramsContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_func_params);
		try {
			setState(54);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PARAM_OUT_ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(48);
				match(PARAM_OUT_ID);
				setState(49);
				match(PARAM_IN_ID);
				setState(50);
				match(T__6);
				setState(51);
				match(TYPE);
				setState(52);
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

	@SuppressWarnings("CheckReturnValue")
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
	}

	public final Func_params_nContext func_params_n() throws RecognitionException {
		Func_params_nContext _localctx = new Func_params_nContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_func_params_n);
		try {
			setState(63);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__7:
				enterOuterAlt(_localctx, 1);
				{
				setState(56);
				match(T__7);
				setState(57);
				match(PARAM_OUT_ID);
				setState(58);
				match(PARAM_IN_ID);
				setState(59);
				match(T__6);
				setState(60);
				match(TYPE);
				setState(61);
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

	@SuppressWarnings("CheckReturnValue")
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
	}

	public final Statement_listContext statement_list() throws RecognitionException {
		Statement_listContext _localctx = new Statement_listContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_statement_list);
		try {
			setState(69);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__8:
			case T__11:
			case T__12:
			case T__13:
			case T__14:
			case LITERAL:
			case VAR_DEFINITION:
			case FUNC_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(65);
				statement();
				setState(66);
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

	@SuppressWarnings("CheckReturnValue")
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
		public TerminalNode VAR_DEFINITION() { return getToken(ifjParser.VAR_DEFINITION, 0); }
		public TerminalNode IDENTIFIER() { return getToken(ifjParser.IDENTIFIER, 0); }
		public Var_definition_valueContext var_definition_value() {
			return getRuleContext(Var_definition_valueContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_statement);
		try {
			setState(98);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,5,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(71);
				match(T__8);
				setState(72);
				expr();
				setState(73);
				match(T__9);
				setState(74);
				statement_list();
				setState(75);
				match(T__10);
				setState(76);
				statement_list();
				setState(77);
				match(T__4);
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(79);
				match(T__11);
				setState(80);
				expr();
				setState(81);
				match(T__9);
				setState(82);
				statement_list();
				setState(83);
				match(T__4);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(85);
				match(T__12);
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(86);
				match(T__13);
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(87);
				match(T__14);
				setState(88);
				optional_expr();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(89);
				match(FUNC_ID);
				setState(90);
				match(T__1);
				setState(91);
				call_params();
				setState(92);
				match(T__2);
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(94);
				match(VAR_DEFINITION);
				setState(95);
				match(IDENTIFIER);
				setState(96);
				var_definition_value();
				}
				break;
			case 8:
				enterOuterAlt(_localctx, 8);
				{
				setState(97);
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

	@SuppressWarnings("CheckReturnValue")
	public static class Var_definition_valueContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public Var_definition_valueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_var_definition_value; }
	}

	public final Var_definition_valueContext var_definition_value() throws RecognitionException {
		Var_definition_valueContext _localctx = new Var_definition_valueContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_var_definition_value);
		try {
			setState(103);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__15:
				enterOuterAlt(_localctx, 1);
				{
				setState(100);
				match(T__15);
				setState(101);
				expr();
				}
				break;
			case T__0:
			case T__4:
			case T__8:
			case T__10:
			case T__11:
			case T__12:
			case T__13:
			case T__14:
			case LITERAL:
			case VAR_DEFINITION:
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

	@SuppressWarnings("CheckReturnValue")
	public static class Optional_exprContext extends ParserRuleContext {
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public Optional_exprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_optional_expr; }
	}

	public final Optional_exprContext optional_expr() throws RecognitionException {
		Optional_exprContext _localctx = new Optional_exprContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_optional_expr);
		try {
			setState(107);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,7,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(105);
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

	@SuppressWarnings("CheckReturnValue")
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
	}

	public final ExprContext expr() throws RecognitionException {
		ExprContext _localctx = new ExprContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_expr);
		try {
			setState(115);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LITERAL:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(109);
				term();
				}
				break;
			case FUNC_ID:
				enterOuterAlt(_localctx, 2);
				{
				setState(110);
				match(FUNC_ID);
				setState(111);
				match(T__1);
				setState(112);
				call_params();
				setState(113);
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

	@SuppressWarnings("CheckReturnValue")
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
	}

	public final Call_paramsContext call_params() throws RecognitionException {
		Call_paramsContext _localctx = new Call_paramsContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_call_params);
		try {
			setState(122);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LITERAL:
			case PARAM_OUT_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(117);
				call_param_kw();
				setState(118);
				term();
				setState(119);
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

	@SuppressWarnings("CheckReturnValue")
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
	}

	public final Call_param_nContext call_param_n() throws RecognitionException {
		Call_param_nContext _localctx = new Call_param_nContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_call_param_n);
		try {
			setState(129);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case LITERAL:
			case PARAM_OUT_ID:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(124);
				call_param_kw();
				setState(125);
				term();
				setState(126);
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

	@SuppressWarnings("CheckReturnValue")
	public static class Call_param_kwContext extends ParserRuleContext {
		public TerminalNode PARAM_OUT_ID() { return getToken(ifjParser.PARAM_OUT_ID, 0); }
		public Call_param_kwContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_call_param_kw; }
	}

	public final Call_param_kwContext call_param_kw() throws RecognitionException {
		Call_param_kwContext _localctx = new Call_param_kwContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_call_param_kw);
		try {
			setState(134);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case PARAM_OUT_ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(131);
				match(PARAM_OUT_ID);
				setState(132);
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

	@SuppressWarnings("CheckReturnValue")
	public static class TermContext extends ParserRuleContext {
		public TerminalNode IDENTIFIER() { return getToken(ifjParser.IDENTIFIER, 0); }
		public TerminalNode LITERAL() { return getToken(ifjParser.LITERAL, 0); }
		public TermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_term; }
	}

	public final TermContext term() throws RecognitionException {
		TermContext _localctx = new TermContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_term);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(136);
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
		"\u0004\u0001\u001a\u008b\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001"+
		"\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004"+
		"\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007"+
		"\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b"+
		"\u0002\f\u0007\f\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001"+
		"\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001"+
		"\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0003\u0000*\b"+
		"\u0000\u0001\u0001\u0001\u0001\u0001\u0001\u0003\u0001/\b\u0001\u0001"+
		"\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0002\u0003"+
		"\u00027\b\u0002\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0003\u0003@\b\u0003\u0001\u0004\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0003\u0004F\b\u0004\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0003\u0005c\b\u0005\u0001\u0006\u0001\u0006\u0001"+
		"\u0006\u0003\u0006h\b\u0006\u0001\u0007\u0001\u0007\u0003\u0007l\b\u0007"+
		"\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0003\bt\b\b\u0001\t"+
		"\u0001\t\u0001\t\u0001\t\u0001\t\u0003\t{\b\t\u0001\n\u0001\n\u0001\n"+
		"\u0001\n\u0001\n\u0003\n\u0082\b\n\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0003\u000b\u0087\b\u000b\u0001\f\u0001\f\u0001\f\u0000\u0000\r\u0000"+
		"\u0002\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u0000\u0001"+
		"\u0002\u0000\u0011\u0011\u0018\u0018\u0090\u0000)\u0001\u0000\u0000\u0000"+
		"\u0002.\u0001\u0000\u0000\u0000\u00046\u0001\u0000\u0000\u0000\u0006?"+
		"\u0001\u0000\u0000\u0000\bE\u0001\u0000\u0000\u0000\nb\u0001\u0000\u0000"+
		"\u0000\fg\u0001\u0000\u0000\u0000\u000ek\u0001\u0000\u0000\u0000\u0010"+
		"s\u0001\u0000\u0000\u0000\u0012z\u0001\u0000\u0000\u0000\u0014\u0081\u0001"+
		"\u0000\u0000\u0000\u0016\u0086\u0001\u0000\u0000\u0000\u0018\u0088\u0001"+
		"\u0000\u0000\u0000\u001a\u001b\u0005\u0001\u0000\u0000\u001b\u001c\u0005"+
		"\u0015\u0000\u0000\u001c\u001d\u0005\u0002\u0000\u0000\u001d\u001e\u0003"+
		"\u0004\u0002\u0000\u001e\u001f\u0005\u0003\u0000\u0000\u001f \u0003\u0002"+
		"\u0001\u0000 !\u0005\u0004\u0000\u0000!\"\u0003\b\u0004\u0000\"#\u0005"+
		"\u0005\u0000\u0000#$\u0003\u0000\u0000\u0000$*\u0001\u0000\u0000\u0000"+
		"%&\u0003\n\u0005\u0000&\'\u0003\u0000\u0000\u0000\'*\u0001\u0000\u0000"+
		"\u0000(*\u0001\u0000\u0000\u0000)\u001a\u0001\u0000\u0000\u0000)%\u0001"+
		"\u0000\u0000\u0000)(\u0001\u0000\u0000\u0000*\u0001\u0001\u0000\u0000"+
		"\u0000+,\u0005\u0006\u0000\u0000,/\u0005\u0014\u0000\u0000-/\u0001\u0000"+
		"\u0000\u0000.+\u0001\u0000\u0000\u0000.-\u0001\u0000\u0000\u0000/\u0003"+
		"\u0001\u0000\u0000\u000001\u0005\u0016\u0000\u000012\u0005\u0017\u0000"+
		"\u000023\u0005\u0007\u0000\u000034\u0005\u0014\u0000\u000047\u0003\u0006"+
		"\u0003\u000057\u0001\u0000\u0000\u000060\u0001\u0000\u0000\u000065\u0001"+
		"\u0000\u0000\u00007\u0005\u0001\u0000\u0000\u000089\u0005\b\u0000\u0000"+
		"9:\u0005\u0016\u0000\u0000:;\u0005\u0017\u0000\u0000;<\u0005\u0007\u0000"+
		"\u0000<=\u0005\u0014\u0000\u0000=@\u0003\u0006\u0003\u0000>@\u0001\u0000"+
		"\u0000\u0000?8\u0001\u0000\u0000\u0000?>\u0001\u0000\u0000\u0000@\u0007"+
		"\u0001\u0000\u0000\u0000AB\u0003\n\u0005\u0000BC\u0003\b\u0004\u0000C"+
		"F\u0001\u0000\u0000\u0000DF\u0001\u0000\u0000\u0000EA\u0001\u0000\u0000"+
		"\u0000ED\u0001\u0000\u0000\u0000F\t\u0001\u0000\u0000\u0000GH\u0005\t"+
		"\u0000\u0000HI\u0003\u0010\b\u0000IJ\u0005\n\u0000\u0000JK\u0003\b\u0004"+
		"\u0000KL\u0005\u000b\u0000\u0000LM\u0003\b\u0004\u0000MN\u0005\u0005\u0000"+
		"\u0000Nc\u0001\u0000\u0000\u0000OP\u0005\f\u0000\u0000PQ\u0003\u0010\b"+
		"\u0000QR\u0005\n\u0000\u0000RS\u0003\b\u0004\u0000ST\u0005\u0005\u0000"+
		"\u0000Tc\u0001\u0000\u0000\u0000Uc\u0005\r\u0000\u0000Vc\u0005\u000e\u0000"+
		"\u0000WX\u0005\u000f\u0000\u0000Xc\u0003\u000e\u0007\u0000YZ\u0005\u0015"+
		"\u0000\u0000Z[\u0005\u0002\u0000\u0000[\\\u0003\u0012\t\u0000\\]\u0005"+
		"\u0003\u0000\u0000]c\u0001\u0000\u0000\u0000^_\u0005\u0012\u0000\u0000"+
		"_`\u0005\u0018\u0000\u0000`c\u0003\f\u0006\u0000ac\u0003\u0010\b\u0000"+
		"bG\u0001\u0000\u0000\u0000bO\u0001\u0000\u0000\u0000bU\u0001\u0000\u0000"+
		"\u0000bV\u0001\u0000\u0000\u0000bW\u0001\u0000\u0000\u0000bY\u0001\u0000"+
		"\u0000\u0000b^\u0001\u0000\u0000\u0000ba\u0001\u0000\u0000\u0000c\u000b"+
		"\u0001\u0000\u0000\u0000de\u0005\u0010\u0000\u0000eh\u0003\u0010\b\u0000"+
		"fh\u0001\u0000\u0000\u0000gd\u0001\u0000\u0000\u0000gf\u0001\u0000\u0000"+
		"\u0000h\r\u0001\u0000\u0000\u0000il\u0003\u0010\b\u0000jl\u0001\u0000"+
		"\u0000\u0000ki\u0001\u0000\u0000\u0000kj\u0001\u0000\u0000\u0000l\u000f"+
		"\u0001\u0000\u0000\u0000mt\u0003\u0018\f\u0000no\u0005\u0015\u0000\u0000"+
		"op\u0005\u0002\u0000\u0000pq\u0003\u0012\t\u0000qr\u0005\u0003\u0000\u0000"+
		"rt\u0001\u0000\u0000\u0000sm\u0001\u0000\u0000\u0000sn\u0001\u0000\u0000"+
		"\u0000t\u0011\u0001\u0000\u0000\u0000uv\u0003\u0016\u000b\u0000vw\u0003"+
		"\u0018\f\u0000wx\u0003\u0014\n\u0000x{\u0001\u0000\u0000\u0000y{\u0001"+
		"\u0000\u0000\u0000zu\u0001\u0000\u0000\u0000zy\u0001\u0000\u0000\u0000"+
		"{\u0013\u0001\u0000\u0000\u0000|}\u0003\u0016\u000b\u0000}~\u0003\u0018"+
		"\f\u0000~\u007f\u0003\u0014\n\u0000\u007f\u0082\u0001\u0000\u0000\u0000"+
		"\u0080\u0082\u0001\u0000\u0000\u0000\u0081|\u0001\u0000\u0000\u0000\u0081"+
		"\u0080\u0001\u0000\u0000\u0000\u0082\u0015\u0001\u0000\u0000\u0000\u0083"+
		"\u0084\u0005\u0016\u0000\u0000\u0084\u0087\u0005\u0007\u0000\u0000\u0085"+
		"\u0087\u0001\u0000\u0000\u0000\u0086\u0083\u0001\u0000\u0000\u0000\u0086"+
		"\u0085\u0001\u0000\u0000\u0000\u0087\u0017\u0001\u0000\u0000\u0000\u0088"+
		"\u0089\u0007\u0000\u0000\u0000\u0089\u0019\u0001\u0000\u0000\u0000\f)"+
		".6?Ebgksz\u0081\u0086";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}
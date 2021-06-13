modded class ChatLine
{
	private const int STATUS_TEXT_COLOUR = ARGB(255, 255, 255, 255);
	private const int IMPORTANT_TEXT_COLOUR = ARGB(255, 255, 0, 0);

	override void Set(ChatMessageEventParams params)
	{
		super.Set(params);

		// set chat colour
		switch (params.param4)
		{
			case "colorStatusChannel":
				SetColour(STATUS_TEXT_COLOUR);
				break;
			case "colorImportant":
				SetColour(IMPORTANT_TEXT_COLOUR);
				break;
			
			default:
				break;
		}
	}
}